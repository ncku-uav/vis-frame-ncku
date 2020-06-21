#!/usr/bin/env python
#the conumer to be usedby every websocketd connection

import pika
import sys
exchange_name='default' if (len(sys.argv) <2) else sys.argv[1];
connection = pika.BlockingConnection(pika.ConnectionParameters(host='127.0.0.1'))
#connection = pika.BlockingConnection(pika.ConnectionParameters(host='http://groundcontrol.fritz.box'))
channel = connection.channel()
 
channel.exchange_declare(exchange=exchange_name,
type='fanout')
 
result = channel.queue_declare(exclusive=True)
queue_name = result.method.queue
 
channel.queue_bind(exchange=exchange_name,
                    queue=queue_name)

def callback(ch, method, properties, body):
	sys.stdout.write(str(body) + '\n');
	sys.stdout.flush();
 
channel.basic_consume(callback,
                    queue=queue_name,
                    no_ack=True)
 
channel.start_consuming() 
