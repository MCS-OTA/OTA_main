# secure_ota_socket_server.py
import socket, os

socket_path = "/tmp/ota_secure.sock"

if os.path.exists(socket_path):
    os.remove(socket_path)

server = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
server.bind(socket_path)
os.chmod(socket_path, 0o600)  # 🔐 1. 접근 권한 제한 (600)

server.listen(1)
print("[Python] OTA secure socket listening...")

conn, _ = server.accept()
print("[Python] Client connected.")

data = conn.recv(1024).decode()

# 🔐 2. 간단한 pre-shared token 인증
if data.startswith("token:secret123|"):
    command = data.split("|", 1)[1]
    print(f"[Python] Auth OK, received command: {command}")
    conn.send(b"ACK: Command accepted.\n")
else:
    print("[Python] Invalid token.")
    conn.send(b"ERROR: Unauthorized.\n")

conn.close()
server.close()
