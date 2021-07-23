# Artificial Data provider for the Aircraft_YEE implementation
# sends artificial data to an specified UDP port

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

# initiate socket and send first message
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # Internet, UDP
try:
    sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
except:
    print('Initial message failed!')



async def get_attitude(drone,tag):
    async for attitude in drone.telemetry.attitude_euler():
        roll = attitude.roll_deg
        pitch = attitude.pitch_deg
        break
    if tag == 1:
        return roll
    elif tag == 2:
        return pitch
        

async def get_position(drone,tag):
    async for position in drone.telemetry.position():
        altitude = position.relative_altitude_m
        latitude = position.latitude_deg
        longitude = position.longitude_deg
        break
    #data={altitude,latitude,longitude}
    #return altitude,latitude,longitude
    if tag == 1:
        return altitude
    elif tag == 2:
        return latitude
    elif tag == 3:
        return longitude
    #return position.relative_altitude_m
    
    
async def get_battery(drone,tag):
    async for battery in drone.telemetry.battery():
        battery_remain = battery.remaining_percent
        battery_volt = battery.voltage_v
        break
    #data = {battery_remain,battery_volt}
    #return battery_remain,battery_volt
    if tag == 1:
        return battery_remain
    elif tag == 2:
        return battery_volt

        

async def run():
    # Init the drone
    # attitude=[]
    # position=[]
    # battery=[]
    drone = System()
    await drone.connect(system_address="udp://:14550")
    try:    
        while True :
            # async for attitude in drone.telemetry.attitude_euler():
            #     roll = attitude.roll_deg
            #     pitch = attitude.pitch_deg
            #     break
            # async for position in drone.telemetry.position():
            #     altitude = position.relative_altitude_m
            #     latitude = position.latitude_deg
            #     longitude = position.longitude_deg
            #     break
            # async for battery in drone.telemetry.battery():
            #     battery_remain = battery.remaining_percent
            #     battery_volt = battery.voltage_v
            #     break
            # async for GPS_INFO in drone.telemetry.gps_info():
            #     gpsinfo = GPS_INFO
            #     break
            #roll=asyncio.ensure_future(get_attitude(drone))

            data = {
                
            "rollVal" : await asyncio.ensure_future(get_attitude(drone,1)),
            #"pitchVal" : get_attitude(drone,2),
            #"yawVal" : cs.yaw,
            "latVal" : await asyncio.ensure_future(get_position(drone,2)),
            "lngVal" : await asyncio.ensure_future(get_position(drone,3)),    
            #"heading" : cs.groundcourse,
            "altVal" : await asyncio.ensure_future(get_position(drone,1)),
            #"altoffsethomeVal" : cs.altoffsethome,
            #"gpsstatusVal" : gpsinfo,   
            #"gpshdopVal" : cs.gpshdop,
            #"satcountVal" : cs.satcount,
            #"altd100Val" : cs.altd100,
            #"altd1000Val" : cs.altd1000,
            #"airspeedVal" : drone.telemetry.FixedwingMetrics.airspeed_m_s,
            #"targetairspeedVal" : cs.targetairspeed,
            #"groundspeedVal" : drone.telemetry.velocity_ned(),
            #"verticalspeedVal" : cs.verticalspeed,
            #"wind_dirVal" : cs.wind_dir,
            #"wind_velVal" : cs.wind_vel,
            #"axVal" : drone.telemetry.VelocityBody.x_m_s, 
            #"ayVal" : drone.telemetry.VelocityBody.y_m_s,
            #"azVal" : drone.telemetry.VelocityBody.z_m_s,
            #"gxVal" : cs.gx,
            #"gyVal" : cs.gy,
            #"gzVal" : cs.gz,
            ### Servo Channels Input
            # "chx1inVal" : cs.chx1in,
            # "chx2inVal" : cs.chx2in,
            # "chx3inVal" : cs.chx3in,
            # "chx4inVal" : cs.chx4in,
            # "chx5inVal" : cs.chx5in,
            # "chx6inVal" : cs.chx6in,
            # "chx7inVal" : cs.chx7in,
            # "chx8inVal" : cs.chx8in,
            ### Servo Channels Output
            # "chx1outVal" : cs.chx1out,
            # "chx2outVal" : cs.chx2out,
            # "chx3outVal" : cs.chx3out,
            # "chx4outVal" : cs.chx4out,
            # "chx5outVal" : cs.chx5out,
            # "chx6outVal" : cs.chx6out,
            # "chx7outVal" : cs.chx7out,
            # "chx8outVal" : cs.chx8out,
            "battery_voltageVal" :await asyncio.ensure_future(get_battery(drone,1)),   
            "battery_remainingVal" :await asyncio.ensure_future(get_battery(drone,2)),
            #"armed" : drone.telemetry.Telemetry.armed   

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

