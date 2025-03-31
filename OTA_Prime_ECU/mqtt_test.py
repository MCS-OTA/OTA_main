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
import tkinter as tk
from utils.OTA_GUI import OTA_GUI

brokerIp = "192.168.86.67"
port = 1883
topic_from_server_notify = "file/added"
topic_from_server_files = "file/files"
topic_to_server = "file/current_json"
topic_permission_client = "permission/client"
topic_permission_server = "permission/server"
unzip_path = "updateFiles"
json_manager = JSON_manager()

def load_updateList():
    print("load_updateList")
    with tarfile.open("received_update.tar.gz", "r:xz") as tar:
        tar.extractall(path=unzip_path)
    json_manager.updateList = json_manager._load_json(unzip_path + "/update.json")
    print("load_updateList success")


def select_time_to_update_with_gui():
    '''
    클래스가 중복 선언되니 나중에 확인 필요
    '''
    while True:
        root = tk.Tk()
        app = OTA_GUI(root)
        root.mainloop()
        if app.selected_time == 0:
            client.publish(topic_permission_server, str(0))
            break
        else:
            for _ in range(app.selected_time):
                time.sleep(1)

def check_and_build_function(event):
    while True:
        event.wait()
        print("\n##### Load Update List #####")
        load_updateList()
        if json_manager.move_original_file_to_tmp():
            print("\n##### Move Original Files to Tmp Success #####")
            if json_manager.move_update_file_to_ws():
                print("\n##### Move Update Files to Workspace Success #####")
                if json_manager.build_update_file(rollback=False):
                    print("\n##### Build Success #####")
                else:
                    print("\n%%%%% Build Failed, Try Roll Back %%%%%")
                    lastVersion = list(json_manager.historyList.keys())[-1]
                    print(f"\n##### Latest Version Found, Latest Version: {lastVersion}")
                    if json_manager.roll_back(lastVersion):
                        print(f"\n##### Roll Back Success, Roll Back to {lastVersion} Version #####")
                        if json_manager.build_update_file(rollback=True):
                            print("\n##### Roll Back Build Success #####")
                        else:
                            print("\n%%%%% Roll Back Build Failed %%%%%")
                    else:
                        print("\n%%%%% Roll Back Failed %%%%%")
            else:
                print("\n%%%%% Move Update Files to Workspace Failed %%%%%")
        else:
            print("\n%%%%% Move Original Files to Tmp Failed %%%%%")
        time.sleep(5)
        event.clear()





def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    client.subscribe(topic_from_server_notify)  # Subscribe to the topic
    client.subscribe(topic_from_server_files)
    client.subscribe(topic_permission_client)

# Callback function when a message is received from the broker
def on_message(client, userdata, msg):
    #print(f"Message received on topic {msg.topic}: {msg.payload.decode()}")
    if msg.topic == topic_from_server_notify:
        print("\n##### New Update Exist Notification From Server #####")
        client.publish(topic_to_server, json.dumps(json_manager.versionList))
        print("\n##### Send Version List to Server #####")
    elif msg.topic == topic_from_server_files:
        print("\n##### New Update Files Have Arrived From Server #####")
        try: 
            with open("received_update.tar.gz", "wb") as f:
                f.write(base64.b64decode(msg.payload))
            print("\n##### New Zip File Saved")
            event.set()
        except:
            print("\n%%%%% New Zip File Save Error %%%%%")
    elif msg.topic == topic_permission_client:
        print("\n##### Server Ask For Permission #####")
        select_time_to_update_with_gui()


        
event = threading.Event()
thread = threading.Thread(target = check_and_build_function, args=(event,), daemon = True)
thread.start()

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(brokerIp, port, 60)

client.loop_start()

for i in range(2000):
	print("#")
	time.sleep(1)
client.loop_stop()
client.disconnect()
