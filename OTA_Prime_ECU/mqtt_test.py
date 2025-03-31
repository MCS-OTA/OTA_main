import paho.mqtt.client as mqtt
import json
import base64
import tarfile
import threading
import time
import sys
from PyQt5.QtWidgets import QApplication
from utils.OTA_GUI import OTA_GUI
from json_manage import JSON_manager

brokerIp = "192.168.86.67"
port = 1883
topic_from_server_notify = "file/added"
topic_from_server_files = "file/files"
topic_to_server = "file/current_json"
topic_permission_client = "permission/client"
topic_permission_server = "permission/server"
unzip_path = "updateFiles"
json_manager = JSON_manager()

event = threading.Event()

def load_updateList():
    print("load_updateList")
    with tarfile.open("received_update.tar.gz", "r:xz") as tar:
        tar.extractall(path=unzip_path)
    json_manager.updateList = json_manager._load_json(unzip_path + "/update.json")
    print("load_updateList success")

def check_and_build_function(event):
    while True:
        event.wait()
        print("\n##### Load Update List #####")
        load_updateList()
        # (중략) → 기존 build 함수 그대로
        time.sleep(5)
        event.clear()

def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    client.subscribe(topic_from_server_notify)
    client.subscribe(topic_from_server_files)
    client.subscribe(topic_permission_client)

def on_message(client, userdata, msg):
    if msg.topic == topic_from_server_notify:
        print("\n##### New Update Exist Notification From Server #####")
        client.publish(topic_to_server, json.dumps(json_manager.versionList))
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
        QTimer.singleShot(0, lambda: show_update_gui(client))

def show_update_gui(client):
    gui = OTA_GUI(app)

    def yes_action():
        print("Permission granted. Start update now.")
        client.publish(topic_permission_server, str(0))
        app.quit()

    def no_action(wait_time):
        print(f"Permission denied. Will ask again in {wait_time} sec.")
        QTimer.singleShot(wait_time * 1000, lambda: show_update_gui(client))
        gui.close()

    gui.on_yes_callback = yes_action
    gui.on_no_callback = no_action
    gui.show()

if __name__ == "__main__":
    thread = threading.Thread(target=check_and_build_function, args=(event,), daemon=True)
    thread.start()

    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(brokerIp, port, 60)
    client.loop_start()

    # PyQt 앱 실행
    from PyQt5.QtCore import QTimer
    app = QApplication(sys.argv)
    sys.exit(app.exec_())
