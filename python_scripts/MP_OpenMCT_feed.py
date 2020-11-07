# Example implementation on how to get (additional) data from MissionPlanner into OpenMCT

import socket
import time

UDP_IP = "127.0.0.1" #localhost
UDP_PORT = 50011 # your telemetry Object port
MESSAGE = "23,567,32,4356,456,132,4353467" # random test message

# set up UDP Port
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # Internet, UDP
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))

i = 0
try:
    while True:
        # Little counter to check for concurrency
        if i == 1000:
            i = 0
        else:
            i += 1

        #choose your values (more available, check MP documentation)

        rollValue = cs.roll
        pitchVal = cs.pitch
        #yawVal = cs.yaw
        #latVal = cs.lat
        #lngVal = cs.lng
        #groundcourseVal = cs.groundcourse
        altVal = cs.alt
        #altoffsethomeVal = cs.altoffsethome
        #gpsstatusVal = cs.gpsstatus
        #gpshdopVal = cs.gpshdop
        #satcountVal = cs.satcount
        #altd100Val = cs.altd100
        #altd1000Val = cs.altd1000
        airspeedVal = cs.airspeed
        #targetairspeedVal = cs.targetairspeed
        #groundspeedVal = cs.groundspeed
        #verticalspeedVal = cs.verticalspeed
        #wind_dirVal = cs.wind_dir
        #wind_velVal = cs.wind_vel
        axVal = cs.ax
        ayVal = cs.ay
        azVal = cs.az
        #gxVal = cs.gx
        #gyVal = cs.gy
        #gzVal = cs.gz
        ### Servo Channels Input
        # chx1inVal = cs.chx1in
        # chx2inVal = cs.chx2in
        # chx3inVal = cs.chx3in
        # chx4inVal = cs.chx4in
        # chx5inVal = cs.chx5in
        # chx6inVal = cs.chx6in
        # chx7inVal = cs.chx7in
        # chx8inVal = cs.chx8in
        ### Servo Channels Output
        # chx1outVal = cs.chx1out
        # chx2outVal = cs.chx2out
        # chx3outVal = cs.chx3out
        # chx4outVal = cs.chx4out
        # chx5outVal = cs.chx5out
        # chx6outVal = cs.chx6out
        # chx7outVal = cs.chx7out
        # chx8outVal = cs.chx8ou

        timeStamp = time.time()

        # Build a message (since we do not have keys implemented here, to parse the message we need to make use of the order in which the data is sent)
        MESSAGE = "{},{},{},{},{},{},{},{},{}".format(pitchVal, rollValue, airspeedVal, altVal, axVal,ayVal,azVal i, timeStamp)
        # Show the timestep
        print(MESSAGE)
        print('\n')

        # Pumping out the values
        sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
        time.sleep(0.01)

except KeyboardInterrupt:
    print("Over")
