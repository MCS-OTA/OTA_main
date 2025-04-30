import subprocess
import re
import os
import sys

def get_arduino_boards():
    try:
        result = subprocess.run(
            ['arduino-cli', 'board', 'list'],
            stdout = subprocess.PIPE,
            stderr = subprocess.PIPE,
            text = True,
            check = True
        )

        output = result.stdout.strip()

        # Set arduino uno info
        pattern = re.compile(
            r'(\/dev\/ttyACM[0-9]+)\s+serial\s+.*Arduino Uno\s+(arduino:avr:uno)\s+arduino:avr'
        )

        # Detect boards
        if output:
            print("#"*20,'\n\nList of Arduino Board:\n\n')
            print(output)
        else:
            print("*"*20, "\n\nNo device detected\n\n")

        match = pattern.search(output)
        if match:
            port = match.group(1)
            fqbn = match.group(2)
            print(f"✅ Board Detected: port={port}, FQBN={fqbn}")
            return port, fqbn
        else:
            print("Can't find any board")
            sys.exit(1)

    except subprocess.CalledProcessError as e:
        print("Excution Error")
        print(e.stderr)
        sys.exit(1)

def compile_sketch(sketch_path, fqbn):
    try:
        print(f"Compiling ... ({sketch_path})")
        subprocess.run(
            ['arduino-cli', 'compile', '--fqbn', fqbn, sketch_path],
            check = True
        )
        print("Compile Success")

    except subprocess.CalledProcessError as e:
        print("Compile Failed")
        sys.exit(1)

def upload_sketch(sketch_path, port, fqbn):
    try:
        print(f"Uploading ... (Port: {port})")
        subprocess.run(
            ['arduino-cli', 'upload', '-p', port, '--fqbn', fqbn, sketch_path],
            check = True
        )
        print("Upload Success")
    except subprocess.CalledProcessError as e:
        print("Upload Failed")
        sys.exit(1)


if __name__ == "__main__":
    sketch_path = "./Arduino/green_light"
    port, fqbn = get_arduino_boards()

    compile_sketch(sketch_path, fqbn)
    upload_sketch(sketch_path, port, fqbn)