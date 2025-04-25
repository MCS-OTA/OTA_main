import sys
import threading
import time
import base64
import json
import tarfile
import ssl
import hashlib
import paho.mqtt.client as mqtt
from utils.OTA_GUI import show_update_gui
from utils.signature.sub_signature import verify_signature
from utils.signature.pub_signature import make_payload_with_signature
from json_manage import JSON_manager

brokerIp = "192.168.86.30"
port = 8883
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

    try:
        session = client._sock
        if session:
            tls_session = session.session
            session_id = client._sock.session.id  # type: bytes
            session_hash = hashlib.sha256(session_id).hexdigest()

            print(f"🔐 TLS Session ID: {tls_session.id.hex()}")
            print(f"🔐 TLS Session ID: {session_id}     ID Hash: {session_hash}")
    except Exception as e:
        print(f"⚠️ Failed to retrieve session ID: {e}")

    client.subscribe(topic_from_server_notify)
    client.subscribe(topic_from_server_files)
    client.subscribe(topic_permission_client)


def on_message(client, userdata, msg):
    if verify_signature(msg.payload):
        if msg.topic == topic_from_server_notify:
            print("\n##### New Update Exist Notification From Server #####")
            payload_target = msg.payload.decode('utf-8')
            data = json.loads(payload_target)
            if "directory" in data:
                json_manager.check_target_is_new(data["directory"])
            else:
                print("\n%%%%% There is No Target Name in MQTT MSG %%%%%")
                return
            version_payload = make_payload_with_signature(json_manager.versionList)
            client.publish(topic_to_server, version_payload)
        elif msg.topic == topic_from_server_files:
            print("\n##### New Update Files Have Arrived From Server #####")
            try:
                file_data = json.loads(msg.payload.decode())

                with open("received_update.tar.gz", "wb") as f:
                    f.write(base64.b64decode(file_data["encoded_files"]))
                print("\n##### New Zip File Saved")
                event.set()
            except:
                print("\n%%%%% New Zip File Save Error %%%%%")
        elif msg.topic == topic_permission_client:
            decoded_payload = json.loads(msg.payload.decode('utf-8'))
            if decoded_payload["reset"]:
                pass
            else:
                print("\n##### Server Ask For Permission #####")
                ask_update_permission(client)
                    
        else:
            print("invalid topic")
    else:
        print("\n##### Verification Fail #####")



def ask_update_permission(client):
    def yes_callback():
        print("Permission granted. Start update now.")
        callback_payload = make_payload_with_signature({"update": True})
        client.publish(topic_permission_server, callback_payload)

    def no_callback(wait_time):
        print(f"Permission denied. Will ask again in {wait_time} sec.")
        timer = threading.Timer(wait_time, lambda: ask_update_permission(client))
        timer.start()

    show_update_gui(yes_callback, no_callback)

# 브로커에 남아있는 retained 메시지를 초기화하는 함수
def clear_retained_message(client, topic):
    client.publish(topic, payload="", qos=0, retain=True)
    print(f"Cleared retained message on topic: {topic}")

def configure_tls(client):
    client.tls_set(
        ca_certs="./utils/certs/ca.crt",
        certfile="./utils/certs/client.crt",
        keyfile="./utils/certs/client.key",
        tls_version=ssl.PROTOCOL_TLSv1_2
    )
    client.tls_insecure_set(False)

if __name__ == "__main__":
    # OTA Build Thread
    build_thread = threading.Thread(target=check_and_build_function, args=(event,), daemon=True)
    build_thread.start()

    # MQTT Client
    client = mqtt.Client()
    configure_tls(client)
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(brokerIp, port, 60)
    clear_retained_message(client, topic_permission_client)
    client.loop_start()

    # Main thread 유지용 (대기)
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        client.loop_stop()
        client.disconnect()

