import os
import time
import json
import paho.mqtt.client as mqtt
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
import base64
import tarfile
import shutil
import ssl
import hashlib
from utils.json_handler import JsonHandler
from utils.signature.pub_signature import make_payload_with_signature
from utils.signature.sub_signature import verify_signature

class FileHandler:
    def __init__(self, mqtt_broker, mqtt_port, watch_dir, files_path):
        self.MQTT_BROKER = mqtt_broker
        self.MQTT_PORT = mqtt_port
        self.WATCH_DIR = watch_dir
        self.files_path = files_path

        self.MQTT_NOTIFY_TOPIC = "file/added"
        self.json_from_client = "file/current_json" #option/option
        self.permission_to_client = "permission/client"
        self.permission_from_client = "permission/server"
        self.rollback_ropic = "rollback"
        self.MQTT_FILE_TOPIC = "file/files"

        self.output_json = "../data/output.json"  # output.json 파일 경로
        self.received_json = "../data/received.json"  # received_data.json 파일 경로
        self.update_json = "../data/update.json"
        self.target_path = "../src_add/"
        self.output_archive = "../data/update.tar.xz"

        self.ca_cert = "./utils/certs/ca.crt"
        self.client_cert = "./utils/certs/client.crt"
        self.client_key = "./utils/certs/client.key"

        self.client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
        configure_tls(self.client, self.ca_cert, self.client_cert, self.client_key)
        
        # MQTT 설정
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

        # 파일 감시 설정
        self.observer = Observer()
        self.event_handler = FileChangeHandler(self.client, self.MQTT_NOTIFY_TOPIC, self.WATCH_DIR)
        self.observer.schedule(self.event_handler, self.WATCH_DIR, recursive=False)

        # jsonmanager
        self.json_handler = JsonHandler()

    def connect_mqtt(self):
        self.client.connect(self.MQTT_BROKER, self.MQTT_PORT, 60)

    def start_watching(self):
        print(f"Watching directory: {self.WATCH_DIR}")
        self.observer.start()

    def stop_watching(self):
        self.observer.stop()

    def loop_mqtt(self):
        self.client.loop_start()


    def encode_files(self, json_path):
        with open(json_path, "rb") as files_file:
            encoded_files = base64.b64encode(files_file.read())
            return encoded_files

    def on_connect(self, client, userdata, flags, rc, topic):
        print(f"Connected: {rc}")
        client.subscribe(self.json_from_client)
        client.subscribe(self.permission_from_client)

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

    def on_message(self, client, userdata, msg):
        if verify_signature(msg.payload):
            payload_data = json.loads(msg.payload.decode('utf-8'))
            
            if msg.topic == "file/current_json" :
                print(f"file/current_json: {msg.payload.decode()}")
                try:
                    json_data = json.loads(msg.payload.decode())
                    keys = list(json_data.keys())

                    if len(keys) >= 4:
                        target_dir = keys[1]
                        final_target_path = os.path.join(self.target_path, target_dir)
                    # JSON 데이터를 파일로 저장
                    with open("../data/received.json", "w", encoding="utf-8") as json_file:
                        json.dump(json_data, json_file, indent=4, ensure_ascii=False)
                    print("Data saved to '../data/received.json'.")

                    self.json_handler.compare_and_update_json(self.output_json, self.received_json, target_dir, self.update_json)
                    self.json_handler.create_update_tarball(self.update_json, final_target_path, self.output_archive)

                except json.JSONDecodeError as e:
                    print(f"Failed to decode JSON: {e}")

                print("=" * 50, "\n\n", "Transfer Update List")
                #update_message = self.encode_files(self.update_json)

                with open(self.update_json, "r") as f:
                    message = json.load(f)

                message["reset"] = False
                update_payload = make_payload_with_signature(message)

                try:
                    result = client.publish(self.permission_to_client, update_payload, qos=1, retain=True)
                    print("Pulbilsh result:  ", result.rc)

                except:
                    print("PUB FAIL")
                print("permission to client sent")

            elif msg.topic =="permission/server":
                print("permission/client: ",payload_data["update"])
                if payload_data["update"]:
                    print("=" * 50, "\n\n", "Update New Files")
                    encoded_files = self.encode_files(self.files_path)
                    file_message = {"encoded_files": encoded_files.decode()}
                    file_payload = make_payload_with_signature(file_message)
                    
                    client.publish(self.MQTT_FILE_TOPIC, file_payload)
                    
                    print("=" * 50, "\n\n", "Reset the Broker")
                    reset_payload = make_payload_with_signature({"reset": True})
                    client.publish(self.permission_to_client, reset_payload, qos=1, retain=True)

                else:
                    pass

        else:
            print("\n##### Verification Fail #####")

class FileChangeHandler(FileSystemEventHandler):
    def __init__(self, client, mqtt_notify_topic, watch_dir):
        self.client = client
        self.MQTT_NOTIFY_TOPIC = mqtt_notify_topic
        self.watch_dir = watch_dir
        self.json_handler = JsonHandler()

    def on_created(self, event):
        if event.is_directory:  # 디렉토리가 생성될 때만 처리
            foldername = os.path.basename(event.src_path)
            print(f"New directory detected: {foldername}")

            # MQTT 메시지 전송
            # message = json.dumps({"event": "directory_added", "directory": foldername})
            print("=" * 50, "\n\n", "Notify New Update")
            message = {"event": "directory_added", "directory": foldername}
            notify_payload = make_payload_with_signature(message)

            self.client.publish(self.MQTT_NOTIFY_TOPIC, notify_payload)

            # 디렉토리 내부의 파일들을 JSON으로 저장
            print(f"Running directory_to_json for {event.src_path}")
            self.json_handler.target_to_json(event.src_path, "../data/output.json")
            print("directory_to_json executed.")

def configure_tls(client, ca_cert, client_cert, client_key):
    client.tls_set(
        ca_certs= ca_cert,
        certfile= client_cert,
        keyfile= client_key,
        tls_version=ssl.PROTOCOL_TLSv1_2
    )
    client.tls_insecure_set(False)

# 사용 예시
if __name__ == "__main__":
    # MQTT 설정
    MQTT_BROKER = "192.168.86.30"  # 또는 MQTT 서버 IP
    MQTT_PORT = 8883

    # 감시할 디렉토리 설정
    WATCH_DIR = "../src_add"  # 감시할 폴더 경로 변경 필요

    # 파일 경로 및 MQTT 클라이언트 설정
    files_path = "../data/update.tar.xz"
    
    # 파일 처리 객체 생성 및 MQTT 연결
    file_handler = FileHandler(MQTT_BROKER, MQTT_PORT, WATCH_DIR, files_path)
    file_handler.connect_mqtt()
    
    # MQTT 및 파일 감시 시작
    file_handler.start_watching()
    file_handler.loop_mqtt()

    try:
        while True:
            print("-")
            time.sleep(1)
    except KeyboardInterrupt:
        file_handler.stop_watching()
        file_handler.client.loop_stop()
    file_handler.observer.join()
