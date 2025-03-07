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

unzip_path = "/home/pdc/OTA_client/updateFiles"
json_manager = JSON_manager()

def check_and_build_function(event):
    event.wait()
    print("extracting")
    with tarfile.open("received_update.tar.gz", "r:xz") as tar:
        tar.extractall(path=unzip_path)
    print("building")
    json_manager.updateList=json_manager._load_json(unzip_path + "/update.json")
    print(json_manager.updateList)
    json_manager.move_original_file_to_tmp()





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
thread = threading.Thread(target = check_and_build_function, args=(event,))
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
