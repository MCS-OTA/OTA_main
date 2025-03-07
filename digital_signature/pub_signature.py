import paho.mqtt.client as mqtt
import base64
import time

from ecdsa import SigningKey, VerifyingKey, NIST384p

def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code == 0:
        print("connected OK")
    else:
        print("Error: Connection fail, Return code =" ,reason_code)

def on_disconnect(client,userdata,flags,rc = 0):
    print(str(rc),end='\n')

def on_publish(client,userdata,mid):
    print("In on_pub call back mid = ", mid,end='\n')

if __name__ == "__main__":
    # MQTT Setting
    client = mqtt.Client(protocol=mqtt.MQTTv5)

    client.on_connect = on_connect
    client.on_disconnect = on_disconnect
    client.on_publish = on_publish

    # Make Signature
    sk = SigningKey.from_pem(open("private.pem").read())
    message = open("./message.txt", "rb").read()

    signature = sk.sign(message)

    # Publish MQTT
    while True:
        client.connect("192.168.86.250", 1883)
        client.loop_start()
        try:
            pub_message = base64.b64encode(signature + b"/" + message)
            print("Publish:    ", pub_message)
            client.publish("OTA", pub_message, 2, retain= True)
            print("Success Publish")
            time.sleep(2)

        except KeyboardInterrupt:
            print("Terminating...")

        
        client.loop_stop()
        client.disconnect()