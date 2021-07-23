import socket
import time
import asyncio
from mavsdk import System




UDP_IP = "127.0.0.1" #standard ip udp (localhost)
UDP_PORT =50020   #chosen port to OpenMCT (same as in telemetry server object)
MESSAGE = "23,567,32,4356,456,132,4353467" #init message

data = 0 #artificial data

keys = [
    # those are the keys for the Aircraft_YEE, which are declared in the dictionary on OpenMCT side
    # since they are not sent, we have ti initialize them here
         "rollVal","pitchVal","yawVal","latVal","lngVal","heading","altVal","gpsstatusVal","airspeedVal","groundspeedVal","azVal","battery_voltageVal","battery_remainingVal"

]

#print(len(topics))

data = 0

# initiate socket and send first message
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # Internet, UDP
try:
    sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
except:
    print('Initial message failed!')


async def run():
    # Init the drone
    drone = System()
    await drone.connect(system_address="udp://:14550")
    try:    
        while True :
            async for attitude in drone.telemetry.attitude_euler():
                roll = attitude.roll_deg
                pitch = attitude.pitch_deg
                break
            async for position in drone.telemetry.position():
                altitude = position.relative_altitude_m
                latitude = position.latitude_deg
                longitude = position.longitude_deg
                break
            async for battery in drone.telemetry.battery():
                battery_remain = battery.remaining_percent
                battery_volt = battery.voltage_v
                break
            async for GPS_INFO in drone.telemetry.gps_info():
                gpsinfo = GPS_INFO
                break
            data = {
                
            "rollVal" : roll,
            "pitchVal" : pitch,
            "latVal" : latitude,
            "lngVal" : longitude,    
            "altVal" : altitude,
            "gpsstatusVal" : gpsinfo,   
            "battery_voltageVal" : battery_volt,   
            "battery_remainingVal" :battery_remain,   
            }


            timeStamp = time.time()
            for key,value in data.items():

                # print("HI I'm here")
                MESSAGE = "{},{},{}".format(key,value, timeStamp)
                print(MESSAGE)
                print('\n')

                # Pumping out the values
                sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
                time.sleep(0.01)
        
    except KeyboardInterrupt:
            print("Over")


if __name__ == "__main__":
    # Start the main function
    asyncio.ensure_future(run())

    # Runs the event loop until the program is canceled with e.g. CTRL-C
    asyncio.get_event_loop().run_forever()