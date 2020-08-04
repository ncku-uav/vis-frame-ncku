import socket
import time


import paho.mqtt.client as mqtt


UDP_IP = "127.0.0.1" #standard ip udp
UDP_PORT = 50012 #chosen port to OpenMCT
MESSAGE = "23,567,32,4356,456,132,4353467" #init message

# initiate socket and send first message
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # Internet, UDP
try:
    sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
except:
    print('Initial message failed!')
    
    
# MQTT    
MQTT_SERVER = "192.168.0.194"
MQTT_PATH = "data/#"
 
# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
 
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe(MQTT_PATH)
 
# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    #print(msg.topic+" "+str(msg.payload))
    # get your data
    timeStamp = time.time()
    topic_formatted = msg.topic.replace("/",".")
 
      
    # Message for OpenMCT must be the same structure as on the receiving side (telemetrysource)
    # especially timestamp needs to be at the same position
    MESSAGE = "{},{},{}".format(topic_formatted,msg.payload.decode("utf-8"),timeStamp)
    

    # Pumping out the values
    sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
      

    #print your message for validation and wait for the next loop 
    print(MESSAGE)
    print('\n')
    time.sleep(0.005)

 
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
 
client.connect(MQTT_SERVER, 1883, 60)
 
# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
