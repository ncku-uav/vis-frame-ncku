import paho.mqtt.client as mqtt #import the client1
import time

broker_address="192.168.0.101" 
#broker_address="iot.eclipse.org" #use external broker
client = mqtt.Client("P1") #create new instance
client.connect(broker_address) #connect to broker
count = 0
while True:
    client.publish("data/apd/pdyn" ,count)#publish
    print(count)
    count = count +1
    time.sleep(0.01)