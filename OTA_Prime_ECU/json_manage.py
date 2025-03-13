#define VERSON 1.0
import json
import os
import shutil
import subprocess
class JSON_manager():
    def __init__(self):
        self.path_versionList = "versionList.json"
        self.path_path_dict = "path_dict.json"
        self.versionList = self._load_json(self.path_versionList)
        self.path_dict = self._load_json(self.path_path_dict)
        self.updateList = None
        self.tmp_path = "tmp"
        self.path_updateFiles = "updateFiles/update_files"
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
        print(f"##### move original files to tmp #####")
        if not os.path.exists(self.updateList["version"]):
            os.makedirs(self.updateList["version"])
            self.tmp_path = self.updateList["version"]
        for target in self.updateList:
            print(f"target = {target}")
            if target == "version":
                pass
            else:
                # move original file to tmp dir
                for file_name in self.updateList[target]:
                    relative_path = self.updateList[target][file_name]["path"]
                    file_path = os.path.join(self.path_dict[target], relative_path)
                    target_path = os.path.join(self.tmp_path, file_name)
                    try:
                        shutil.copy2(file_path, target_path)
                        print(f"{file_path} -> {target_path}")
                    except:
                        print("error occur")
                        return False
        return True
                    
    def move_update_file_to_ws(self):
        print(f"##### move update file to workspace #####")
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
                        print("error occur")
                        return False
        return True
    
    def build_update_file(self):
        print(f"##### build update file #####")
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
            else:
                print("build not ok")
