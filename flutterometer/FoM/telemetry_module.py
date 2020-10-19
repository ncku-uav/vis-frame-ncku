# Flutterometer telemetry
# send some shit
# list ports: python -m serial.tools.list_ports

import serial
import struct
import json
#from flutterometer_telemetry import *

def telemetry_module(sys):
	comPort = '/dev/ttyUSB0'
	count = 0

	# init serial connection to 3dr module
	ser = serial.Serial(comPort, 57600, timeout=30)
	print(ser.name)

	# overview on what is sent
	# sys = flutterometer_telemetry(count)
	payload = sys.sys.tobytes()
	size = sys.sys.shape
	byteSize = struct.pack('hhi',size[0],size[1],len(payload))
	print('Payload length: ', len(payload),'Array size: ', size)
	print('Payload example: ', sys.sys)



	#while True:

	#sys = flutterometer_telemetry(count)
	payload = sys.tobytes()
	size = sys.shape
	byteSize = struct.pack('hhh',size[0],size[1],len(payload))

	ser.write(b's') #startbyte
	ser.write(byteSize) #size information
	ser.write(payload) #payload
 	ser.write(b'e') #endbyte

	
