import bluetooth

server_sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
server_sock.bind(("", 1))
server_sock.listen(1)

# port = server_sock.getsockname()[1]

# bluetooth.advertise_service(server_sock, "RPiServer", service_id="00001101-0000-1000-8000-00805F9B34FB", service_classes=["00001101-0000-1000-8000-00805F9B34BF"], profiles=[bluetooth.SERIAL_PORT_PROFILE])
print("Waiting for connection...")
client_sock, address = server_sock.accept()
print(f"Connected to {address}")

try:
    while True:
        data = client_sock.recv(1024)
        if not data:
            print("Disconnected.")
            break

        message = data.decode('utf-8')
        print("Received:", message)
        client_sock.send("ACK".encode('utf-8'))

        user_input = input("Send to client (or press Enter to skip): ")
        if user_input:
            client_sock.send(user_input.encode('utf-8'))
            print("Sent: ", user_input)

except Exception as e:
    print("Error: ", e)

finally:
    client_sock.close()
    server_sock.close()
    print("Connection closed")
