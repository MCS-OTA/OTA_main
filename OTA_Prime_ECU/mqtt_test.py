import paho.mqtt.client as mqtt
import os
import shutil
import subprocess
import threading
import time
import base64
import json
import tarfile
from json_manage import JSON_manager

brokerIp = "192.168.86.67"
port = 1883
topic_from_server_notify = "file/added"
topic_from_server_files = "file/files"
topic_to_server = "option/option"

unzip_path = "updateFiles"
json_manager = JSON_manager()

def load_updateList():
    print("load_updateList")
    with tarfile.open("received_update.tar.gz", "r:xz") as tar:
        tar.extractall(path=unzip_path)
    json_manager.updateList = json_manager._load_json(unzip_path + "/update.json")
    print("load_updateList success")

def check_and_build_function(event):
    while True:
        event.wait()
        print("extracting")
        load_updateList()
        if json_manager.move_original_file_to_tmp():
            print("move_original_file_to_tmp_success")
            if json_manager.move_update_file_to_ws():
                print("move_update_file_to_ws_success")
                json_manager.build_update_file()
            else:
                print("move_update_file_to_ws_fail")
        else:
            print("move_original_file to tmp fail")
        time.sleep(5)
        event.clear()





def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    client.subscribe(topic_from_server_notify)  # Subscribe to the topic
    client.subscribe(topic_from_server_files)

# Callback function when a message is received from the broker
def on_message(client, userdata, msg):
    #print(f"Message received on topic {msg.topic}: {msg.payload.decode()}")
    if msg.topic == topic_from_server_notify:
        print("send data")
        client.publish(topic_to_server, json.dumps(json_manager.versionList))
    elif msg.topic == topic_from_server_files:
        print("new zip for update file arrived")
        try: 
            with open("received_update.tar.gz", "wb") as f:
                f.write(base64.b64decode(msg.payload))
            print("new zip saved")
            event.set()
        except:
            print("save error")
        
event = threading.Event()
thread = threading.Thread(target = check_and_build_function, args=(event,), daemon = True)
thread.start()

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(brokerIp, port, 60)

client.loop_start()

for i in range(2000):
	print(i)
	time.sleep(1)
client.loop_stop()
client.disconnect()
