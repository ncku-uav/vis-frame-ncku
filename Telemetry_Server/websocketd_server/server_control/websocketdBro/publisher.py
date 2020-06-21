#!/usr/bin/env python
#-This is thepublisher part of Bro
#-It accepts exhange name  and rouring as well
#-as a message template as arguments and the 
#-message to publish from stdin
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!#
#**********Caution for exchange name argv[1]:#
#default exchange is called default
#asuming that you have no other exchanges
#already running called default for other 
#purposes you could run the Bro without arguments
#but this is not recommended!
import pika
import sys
#message= sys.stdin.read().rstrip()

exchange_name=sys.argv[1] or 'default';
#just in cas in the future we don't use fanout
if len(sys.argv)>2:
	routing_key=sys.argv[2] or '';
else:
        routing_key='';


connection = pika.BlockingConnection(pika.ConnectionParameters(
#host='groundcontrol.fritz.box'))
host='127.0.0.1'))
channel = connection.channel()
channel.exchange_declare(exchange=exchange_name,
                    type='fanout')

while(1):
	message= sys.stdin.readline().rstrip()
#	print message;
	channel.basic_publish(exchange=exchange_name, routing_key=routing_key, body=message)


connection.close() 
