import serial
import struct
import json

def tel_mod(sys):
	comPort = '/dev/ttyUSB0'
	ids = 20

	ser = serial.Serial(comPort, 57600, timeout=30)
	print(ser.name)

	#payload = sys.tobytes()
	#size = sys.shape
	#byteSize = struct.pack('hhh', size[0],size[1],len(payload))
	#print('Payload length: ', len(payload), 'Array size: ', size)
	#print('Payload example: ', sys)

	#ser.write(b's')
	#ser.write(byteSize)
	#ser.write(payload)
	#ser.write(b'e')


	## new json message format for telemetry server
	for i in range(ids):
		payload = {"FLIPASED": {"id": str(sys[i,3]), "freq": str(sys[i,1]), "damp": str(sys[i,2]), "timest": str(sys[i,0])}}
		ser.write(json.dumps(payload).encode())
		ser.write('\n'.encode())
		#print(json.dumps(payload))
