#define VERSON 1.0
import json
import os
import shutil
class JSON_manager():
    def __init__(self):
        self.path_versionList = "/home/pdc/OTA_client/versionList.json"
        self.path_path_dict = "/home/pdc/OTA_client/path_dict.json"
        self.versionList = self._load_json(self.path_versionList)
        self.path_dict = self._load_json(self.path_path_dict)
        self.updateList = None
        self.tmp_path = "/home/pdc/OTA_client/tmp"
    def _load_json(self, json_path):
        try:
            with open(json_path, "r") as f:
                data = json.load(f)
            print(f"json file load ok: {json_path}")
            return data
        except FileNotFoundError:
            print(f"file not found : {json_path}")
            return {}
        except json.JSONDecodeError:
            print(f"json decode error : {json_path}")
            return {}
    def move_original_file_to_tmp(self):
        if not os.path.exists(self.tmp_path):
            os.makedirs(self.tmp_path)
        for target in self.updateList:
            print(f"target = {target}")
            if target == "version":
                pass
            else:
                for file_name in self.updateList[target]:
                    relative_path = self.updateList[target][file_name]["path"]
                    #print(f"relative path: {relative_path}")
                    file_path = os.path.join(self.path_dict[target], relative_path)
                    print(f"path_dict = {self.path_dict[target]}")
                    print(f"file_path = {file_path}")
                    target_path = os.path.join(self.tmp_path, file_name)
                    print(file_path)
                    print(target_path)
                    try:
                        shutil.copy2(file_path, target_path)
                        print(f"{file_path} -> {target_path}")
                    except:
                        print("error occur")
