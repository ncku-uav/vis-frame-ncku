from pymavlink import mavutil
import socket
import time

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
the_connection = mavutil.mavlink_connection('udpin:localhost:14551')

# Wait for the first heartbeat 
#   This sets the system and component ID of remote system for the link
the_connection.wait_heartbeat()
print("Heartbeat from system (system %u component %u)" % (the_connection.target_system, the_connection.target_system))
time.sleep(2)
# Once connected, use 'the_connection' to get and send messages

i = 0
run = True
while run:
    try:
        start = time.time()
        GPS_i = the_connection.recv_match(type='GLOBAL_POSITION_INT', blocking=True)  # Note, you can access message fields as attributes!
        GPS_raw = the_connection.recv_match(type='GPS_RAW_INT', blocking=True)
        epoch_time = time.time()
        timestamp = time.ctime(epoch_time)
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
        MESSAGE = "{}".format(GPS_i.relative_alt/1000)


        # Pumping out the values
        sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
        end = time.time()        
        # Show the timestep
        
        # print("{}\t{}\t{}\t{}\t{}".format(GPS_raw.vel/100, GPS_i.relative_alt/1000, i, timestamp,end-start))
        print(MESSAGE)
        print('\n')

        

        
    
    
    except:
        print('Nope, try again!')
        run = False
            
