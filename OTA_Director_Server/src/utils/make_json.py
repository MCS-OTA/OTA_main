import os
import json

def directory_to_json(root_dir, output_file):
    result = {}
    # TestABC 디렉토리 이름을 제외한 경로를 계산하기 위해 상대 경로를 계산
    base_dir = os.path.basename(root_dir)
    
    for dirpath, _, filenames in os.walk(root_dir):
        if "build" in dirpath.split(os.sep):
            continue
        
        for filename in filenames:
            # base_dir을 제외한 상대 경로를 계산
            rel_path = os.path.relpath(os.path.join(dirpath, filename), root_dir)
            result[filename] = {"path": rel_path, "version": 0}
    
    # 최상위에 'version' key를 추가
    final_result = {
        "version": "v0.0.0",  # version key를 최상위에 추가
        base_dir: result  # TestABC 디렉토리 내부의 내용 저장
    }
    
    # JSON 파일로 저장
    with open(output_file, "w", encoding="utf-8") as f:
        json.dump(final_result, f, indent=4, ensure_ascii=False)

# 사용 예시
root_directory = "/home/seame/ota_publisher/publisher/TestABC"  # 탐색할 디렉토리 경로
output_json_file = "output.json"  # 출력할 JSON 파일 이름
directory_to_json(root_directory, output_json_file)
