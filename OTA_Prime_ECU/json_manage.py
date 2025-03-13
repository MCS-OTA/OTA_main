#define VERSON 1.0
import json
import os
import shutil
import subprocess
from datetime import datetime
class JSON_manager():
    def __init__(self):
        self.path_versionList = "versionList.json"
        self.path_path_dict = "path_dict.json"
        self.path_historyList = "history/history.json"
        self.versionList = self._load_json(self.path_versionList)
        self.path_dict = self._load_json(self.path_path_dict)
        self.updateList = None
        self.tmp_path = "history"
        self.path_updateFiles = "updateFiles/update_files"
        self.path_updateList = "updateFiles/update.json"
        self.historyList = self._load_json(self.path_historyList)
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

    def update_versionList(self):
        pass


    def move_original_file_to_tmp(self):
        print(f"\n##### move original files to tmp #####")
        tmp_path = os.path.join(self.tmp_path, self.versionList["version"])
        if not os.path.exists(tmp_path):
            os.makedirs(tmp_path)
        path_backup_version_list = os.path.join(tmp_path, "backup_list.json") 
        shutil.copy2(self.path_updateList, path_backup_version_list)
        backup_version_list = self._load_json(path_backup_version_list)
        for target in self.updateList:
            print(f"target = {target}")
            if target == "version":
                pass
            else:
                # move original file to tmp dir
                for file_name in self.updateList[target]:
                    relative_path = self.updateList[target][file_name]["path"]
                    file_path = os.path.join(self.path_dict[target], relative_path)
                    target_path = os.path.join(tmp_path, file_name)
                    try:
                        shutil.copy2(file_path, target_path)
                        print(f"{file_path} -> {target_path}")
                        backup_version_list[target][file_name]["version"] = self.versionList[target][file_name]["version"]
                    except:
                        print("error occur while moving original files to tmp")
                        return False
        backup_version_list["version"] = self.versionList["version"]
        self.historyList[self.versionList["version"]] = {"changed2" : self.updateList["version"], "date" : datetime.now().isoformat()}
        with open(path_backup_version_list, "w", encoding = "utf-8") as file:
            json.dump(backup_version_list, file, indent=4, ensure_ascii=False)
        with open(self.path_historyList, "w", encoding = "utf-8") as file:
            json.dump(self.historyList, file, indent = 4, ensure_ascii=False)
        return True
                    
    def move_update_file_to_ws(self):
        print(f"\n##### move update file to workspace #####")
        for target in self.updateList:
            print(f"target = {target}")
            if target == "version":
                pass
            else:
                for file_name in self.updateList[target]:
                    relative_path = self.updateList[target][file_name]["path"]
                    target_path = os.path.join(self.path_dict[target],relative_path)
                    file_path = os.path.join(self.path_updateFiles, file_name)
                    try:
                        shutil.copy2(file_path, target_path)
                        print(f"{file_path} -> {target_path}")
                    except:
                        print("error occur while moving update files to ws")
                        return False
        return True
    
    def build_update_file(self):
        print(f"\n##### build update file #####")
        for target in self.updateList:
            if target == "version":
                continue
            build_dir = os.path.join(self.path_dict[target],"build")
            if os.path.exists(build_dir):
                shutil.rmtree(build_dir)    
            os.makedirs(build_dir)
            print(f"build_dir {build_dir}")
            subprocess.run(["cmake", ".."], cwd=build_dir, check=True)
            result = subprocess.run(["make", "-j4"], cwd=build_dir, capture_output=True, text=True)
            if result.returncode == 0:
                print("build ok")
                return True
            else:
                print("build not ok")
                return False

    def roll_back(self, version):
        print(f"\n##### roll back ####")
        path_backup_version_list = os.path.join(self.tmp_path, version,"backup_list.json")
        backup_version_list = self._load_json(path_backup_version_list)
        for target in backup_version_list:
            print(f"target = {target}")
            if target == "version":
                pass
            else:
                for file_name in backup_version_list[target]:
                    relative_path = backup_version_list[target][file_name]["path"]
                    target_path = os.path.join(self.path_dict[target], relative_path)
                    file_path = os.path.join(self.tmp_path, version, file_name)
                    try:
                        shutil.copy2(file_path, target_path)
                        print(f"{file_path} -> {target_path}")
                    except:
                        print("roll back error")
                        return False
        return True

