import serial
import sys
import numpy as np
import time
import struct
from message_parser import parser
from checksum_calculator import calcChecksum
from command_message import getCommandMsg
from OpenMCT_send import sendtoOMCT

# which com port
comPort = '/dev/ttyUSB0'
baudrate = 57600


IDs = {
    0 : 16, #ErrorFlag: ID:0, payload length: 1
    7 : 10, #Battery
    14: 67, #XSENS
    21: 18, #ADS: ID:21, payload length:18
    28: 34, #PPM
    35: 1, #DD
    42: 28, #ECU
    49: 44, #SERVO_REF
    56: 98, #IMU
    63: 58 #SHM
}

#init serial connection to telemetry module
ser = serial.Serial(comPort, baudrate, timeout=30)
print(ser.name)

message = getCommandMsg()

print(message)
ser.write(message)
time.sleep(1)
count = 0

while True:
    #try:
                
        x=0
        variableBits=''
        z = ser.read(1)
        checksum = 0
        h = struct.unpack('B', z)[0]
        payload = ()
        #print(h)
        if h == 150:
            #print('catched!')

            configID = struct.unpack('B', ser.read(1))[0]
            #print('Config ID: ' + str(ConfigID))
            msgID = struct.unpack('B', ser.read(1))[0]
            #print('Msg ID: ' + str(MsgID))
            
            for i in range(IDs[msgID]):
                variableBits = variableBits+'B'
                #print(variableBits)
            
            payload = struct.unpack(variableBits, ser.read(IDs[msgID]))
            #print(payload)
                
            check = ser.read(1)
            head_payload = (150,configID,msgID)+payload
            #print(head_payload)
  
            if calcChecksum(head_payload):
                payload = np.array(payload,dtype=np.uint8)
                data = parser(configID, msgID, payload)
                #print(data)
                sendtoOMCT(data, time.time())
                
    #except:
        #print('fail!')         