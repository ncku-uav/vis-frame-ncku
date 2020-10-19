import serial
import struct

def tel_mod(sys):
	comPort = '/dev/ttyUSB0'
	count = 0

	ser = serial.Serial(comPort, 57600, timeout=30)
	print(ser.name)

	payload = sys.tobytes()
	size = sys.shape
	byteSize = struct.pack('hhh', size[0],size[1],len(payload))
	print('Payload length: ', len(payload), 'Array size: ', size)
	print('Payload example: ', sys)

	ser.write(b's')
	ser.write(byteSize)
	ser.write(payload)
	ser.write(b'e')
