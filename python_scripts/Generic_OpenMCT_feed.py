import socket
import time


UDP_IP = "127.0.0.1" #standard ip udp
UDP_PORT = 50011 #chosen port to OpenMCT
MESSAGE = "23,567,32,4356,456,132,4353467" #init message

# initiate socket and send first message
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # Internet, UDP
try:
    sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
except:
    print('Initial message failed!')


i = 0
run = True
while run:
    try:
        
        # get your data
        data1 = 1+i
        data2 = i*i
        data3 = i*i*i/(i+3)
        timeStamp = time.time()
     
        if i == 1000:
            i = 0
        else:
            i += 1
            
        # Message for OpenMCT must be the same structure as on the receiving side (telemetrysource)
        # especially timestamp needs to be at the same position
        MESSAGE = "{},{},{},{},{},{},{},{},{}".format(data1,data2,data3,data1,data2,data3,data1,data2,timeStamp)
        

        # Pumping out the values
        sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
          

        #print your message for validation and wait for the next loop 
        print(MESSAGE)
        print('\n')
        time.sleep(0.005)

        
        
    
    except:
        print('Nope, try again!')
        run = False
            
