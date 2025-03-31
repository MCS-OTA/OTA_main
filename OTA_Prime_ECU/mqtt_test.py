import sys
import threading
import time
import base64
import json
import tarfile
import paho.mqtt.client as mqtt
from PyQt5.QtWidgets import QApplication
from PyQt5.QtCore import QTimer, QObject, pyqtSignal

from utils.OTA_GUI import OTA_GUI
from json_manage import JSON_manager

brokerIp = "192.168.86.182"
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
        # (생략) 기존 파일 이동 및 빌드 작업
        time.sleep(5)
        event.clear()


class MessageHandler(QObject):
    show_gui_signal = pyqtSignal()

    def __init__(self):
        super().__init__()


def mqtt_thread_function(handler):
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
            handler.show_gui_signal.emit()

    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(brokerIp, port, 60)
    client.loop_forever()


def show_update_gui(client, app):
    gui = OTA_GUI(app)

    def yes_action():
        print("Permission granted. Start update now.")
        client.publish(topic_permission_server, str(0))
        app.quit()

    def no_action(wait_time):
        print(f"Permission denied. Will ask again in {wait_time} sec.")
        QTimer.singleShot(wait_time * 1000, lambda: show_update_gui(client, app))
        gui.close()

    gui.on_yes_callback = yes_action
    gui.on_no_callback = no_action
    gui.show()


def main():
    app = QApplication(sys.argv)
    handler = MessageHandler()

    # OTA Build Thread
    build_thread = threading.Thread(target=check_and_build_function, args=(event,), daemon=True)
    build_thread.start()

    # MQTT Thread
    mqtt_thread = threading.Thread(target=mqtt_thread_function, args=(handler,), daemon=True)
    mqtt_thread.start()

    # Signal 연결
    handler.show_gui_signal.connect(lambda: show_update_gui(mqtt_thread_function.client, app))

    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
