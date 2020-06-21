from pymavlink import mavutil
import socket
import time
import random

def recv_match(self, condition=None, type=None, blocking=False, timeout=None):
    '''Receive the next MAVLink message that matches the given type and condition
    type:        Message name(s) as a string or list of strings - e.g. 'SYS_STATUS'
    condition:   Condition based on message values - e.g. 'SYS_STATUS.mode==2 and SYS_STATUS.nav_mode==4'
    blocking:    Set to wait until message arrives before method completes. 
    timeout:     ? <!-- timeout for blocking message when the system will return. Is this just a time? -->
    '''

UDP_IP = "127.0.0.1"
UDP_PORT = 50011
MESSAGE = "23,567,32,4356,456,132,4353467"

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # Internet, UDP
sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))

# Start a connection listening to a UDP port


i = 0
run = True
while run:
    try:
        start = time.time()

        # print('Velocity: ', GPS_raw.vel/100, 'm/s')
        # print('Altitude: ', GPS_i.relative_alt/1000, 'm')
        # print('Timestamp: ', timestamp)

        
        if i == 1000:
            i = 0
        else:
            i += 1
            
        # Build a message
        # MESSAGE = "{},{},{},{}".format(GPS_raw.vel/100, GPS_i.relative_alt/1000, i, epoch_time)
        # Message for OpenMCT
        MESSAGE = "{},{},{},{},{}".format(i, i*start, i + random.randint(0, 100), i-start, i/start)
        #MESSAGE = "{}".format(Att.pitch*180/3.1415926)


        # Pumping out the values
        sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
        end = time.time()        
        # Show the timestep
        
        # print("{}\t{}\t{}\t{}\t{}".format(GPS_raw.vel/100, GPS_i.relative_alt/1000, i, timestamp,end-start))
        print(MESSAGE,i, end)
        print('\n')

        time.sleep(0.010)

        

        

        
    
    
    except:
        print('Nope, try again!')
        run = False
            
