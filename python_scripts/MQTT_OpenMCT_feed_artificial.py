import socket
import time
#import ntplib


import paho.mqtt.client as mqtt


UDP_IP = "127.0.0.1" #standard ip udp
UDP_PORT = 50012 #chosen port to OpenMCT
MESSAGE = "23,567,32,4356,456,132,4353467" #init message

data = 0 #artificial data

topics = [
        "data.gps.iTOW",
        "data.gps.lon",
        "data.gps.lat" ,
        "data.gps.heightMS" ,
        "data.gps.gSpeed",
        "data.gps.headingMotion" ,
        "data.gps.headingVehicle" ,
		"data.gps.fixType" ,

        "data.nano.vdot" ,
        "data.nano.v" ,
		"data.nano.ch1" ,
		"data.nano.ch2" ,
		"data.nano.ch3" ,
		"data.nano.ch4" ,
		"data.nano.ch5" ,
		"data.nano.ch6" ,
		"data.nano.ch7" ,
		"data.nano.ch8" ,
		"data.nano.ch9" ,
		"data.nano.ch10" ,
		"data.nano.ch11" ,
		"data.nano.ch12" ,
		"data.nano.ch13" ,

        "data.strap.roll",
        "data.strap.pitch" ,
        "data.strap.yaw" ,

        "data.thr.force1" ,
        "data.thr.force2" ,
		"data.thr.temp1" ,
		"data.thr.temp2" ,

		"data.imu.AccX" ,
		"data.imu.AccY" ,
		"data.imu.AccZ" ,
		"data.imu.GyroX" ,
		"data.imu.GyroY" ,
		"data.imu.GyroZ" ,

		"data.adp.pstat" ,
		"data.adp.pdyn" ,
		"data.adp.AirSpeed"

]

print(len(topics))

# initiate socket and send first message
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # Internet, UDP
try:
    sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
except:
    print('Initial message failed!')

#ntp_client = ntplib.NTPClient()

while True:

    for i in range(len(topics)):
        #print(i)
        #response = ntp_client.request('pool.ntp.org')
        #timeStamp = response.tx_time
        timeStamp = time.time()
        MESSAGE = "{},{},{}".format(topics[i],data+i,timeStamp)
        # Pumping out the values
        sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
        #print your message for validation and wait for the next loop
        #print(MESSAGE)
        #print('\n')

    if data < 2000:
        data = data + 1
    else:
        data = 0
    print(data)



    # Message for OpenMCT must be the same structure as on the receiving side (telemetrysource)
    # especially timestamp needs to be at the same position
    # for simulation of DG800 Bandwith, 40 Values at ca. 10Hz -> 400Hz
    time.sleep(0.0020) #considering time needed by python script
