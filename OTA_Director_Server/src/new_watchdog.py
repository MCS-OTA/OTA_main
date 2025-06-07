import os
import time
import json
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
import base64
import shutil
from utils.json_handler import JsonHandler
from utils.signature.pub_signature import make_payload_with_signature
from utils.signature.sub_signature import verify_signature

class FileHandler:
    def __init__(self, watch_dir, image_dir):
        self.WATCH_DIR = watch_dir
        self.IMAGE_DIR = image_dir

        # Set the watchdog
        self.observer = Observer()
        self.event_handler = FileChangeHandler(self.WATCH_DIR, self.IMAGE_DIR)
        self.observer.schedule(self.event_handler, self.WATCH_DIR, recursive=False)

    def start_watching(self):
        print(f"Watching directory: {self.WATCH_DIR}")
        self.observer.start()
    
    def stop_watching(self):
        self.observer.stop()

    def encode_files(self, json_path):
        with open(json_path, "rb") as files_file:
            encoded_files = base64.b64encode(files_file.read())
            return encoded_files

class FileChangeHandler(FileSystemEventHandler):
    def __init__(self, watch_dir, image_dir):
        self.watch_dir = watch_dir
        self.image_dir = image_dir
        self.json_handler = JsonHandler()

    def on_created(self, event):
        if event.is_directory:  # 디렉토리가 생성될 때만 처리
            folder_name = os.path.basename(event.src_path)
            print(f"New directory detected: {folder_name}")

            # MQTT 메시지 전송
            # message = json.dumps({"event": "directory_added", "directory": foldername})
            print("=" * 50, "\n\n", "Notify New Update")

            # 디렉토리 내부의 파일들을 JSON으로 저장
            print(f"Running directory_to_json for {event.src_path}")
            self.json_handler.target_to_inventory_json(event.src_path, "../../OTA_Director_director/data/target_new.json")
            self.json_handler.target_to_json(event.src_path, "../data/target_new.json")
            # with open("../data/output.json", "r", encoding="utf-8") as f:
            #     data = json.load(f)

            dest_path = os.path.join(self.image_dir, folder_name)
            try:
                shutil.copytree(event.src_path, dest_path)
                print(f"Trasfer files to Image Repo    ->    {dest_path}")
            except Exception as e:
                print(f"Fail to transfer:   {e}")
            
            print("directory_to_json executed.")

if __name__ == "__main__":
    # 감시할 디렉토리 설정
    WATCH_DIR = "../src_add"  # 감시할 폴더 경로
    IMAGE_DIR = "../Image_Repo"

    # 파일 처리 객체 생성 및 MQTT 연결
    file_handler = FileHandler(WATCH_DIR, IMAGE_DIR)

    # MQTT 및 파일 감시 시작
    file_handler.start_watching()

    try:
        while True:
            print("-")
            time.sleep(1)
    except KeyboardInterrupt:
        file_handler.stop_watching()
    file_handler.observer.join()