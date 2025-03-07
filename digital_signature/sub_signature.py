import paho.mqtt.client as mqtt
import base64
from ecdsa import VerifyingKey


MainTopic = "OTA"
brokerIp = "192.168.86.250"
port = 1883

def on_connect(client, userdata, flags, reasonCode):
    if reasonCode == 0:
        print("Connected successfully.")
        client.subscribe(MainTopic)
    else:
        print(f"Failed to connect, return code {reasonCode}")

def on_disconnect(client,userdata,flags,rc = 0):
    print(str(rc)+'/')


def on_message(client, userdata, msg):
    try:
        raw_data= base64.b64decode(msg.payload)
        msgList = raw_data.split(b"/")
        signature = msgList[0]
        message = msgList[1]

        with open("../ecdsa/public.pem", "rb") as f:
            vk = VerifyingKey.from_pem(f.read())
            vk.verify(signature, message)
        print("Received message:", message)
        print("on topic", msg.topic)
    except Exception as e:
        print("Error processing signature:", e)



client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(brokerIp,port, keepalive=60)

client.loop_forever()
