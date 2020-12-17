# python3.6

import random
import json

from paho.mqtt import client as mqtt_client
import paho.mqtt.client as mqtt


BROKER = 'broker.emqx.io'
PORT = 1883
CENTRAL_TOPIC = "fse2020/160029503/dispositivos"


def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    client.subscribe("$SYS/#")

def subscribe(client: mqtt_client, topic):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        if(msg.topic == CENTRAL_TOPIC):
            json_load = json.loads(msg.payload.decode())
            choosen_comodo = "sala"
            response = {"comodo":choosen_comodo}
            result = client.publish("fse2020/160029503/dispositivos/" + json_load["id"], json.dumps(response))
            status = result[0]
            if status == 0:
                print(f"Send `{json.dumps(response)}` to topic `{topic}`")
            else:
                print(f"Failed to send message to topic {topic}")

    client.subscribe(topic)
    client.on_message = on_message


def mqtt_init_all():
    client = mqtt.Client()
    client.on_connect = on_connect
    # client.on_message = on_message

    client.connect(BROKER, PORT, 60)

    subscribe(client, CENTRAL_TOPIC)
    client.loop_forever()
