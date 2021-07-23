# Artificial Data provider for the Aircraft_YEE implementation
# sends artificial data to an specified UDP port

import socket
import time
import asyncio
from mavsdk import System



UDP_IP = "127.0.0.1"
# UDP_IP = "192.168.137.1" #standard ip udp (localhost)
UDP_PORT =50020   #chosen port to OpenMCT (same as in telemetry server object)
MESSAGE = "23,567,32,4356,456,132,4353467" #init message


data = 0 #artificial data

keys = [
    # those are the keys for the Aircraft_YEE, which are declared in the dictionary on OpenMCT side
    # since they are not sent, we have ti initialize them here
         "rollVal","pitchVal","yawVal","latVal","lngVal","altVal","airspeedVal","azVal","battery_voltageVal","battery_remainingVal"
         ,"throttle_percentage_Val","climb_rate_Val","velocity_Val","A_altitude","absolute_pressure","differential_pressure_temperature_deg"

]

#print(len(topics))

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
    # await drone.connect(system_address="serial://COM5:115200")
    asyncio.ensure_future(get_attitude(drone))
    asyncio.ensure_future(get_position(drone))
    asyncio.ensure_future(get_battery(drone))
    asyncio.ensure_future(get_speed(drone))
    # asyncio.ensure_future(get_battery(drone))




async def get_attitude(drone):
    while True:
        async for attitude in drone.telemetry.attitude_euler():
            roll = attitude.roll_deg
            pitch =  attitude.pitch_deg
            yaw = attitude.yaw_deg
            break
        data = {               
            "rollVal":roll,
            "pitchVal":pitch,
            "yawVal":yaw,
        }
        timeStamp = time.time()
        for key,value in data.items():
            # print("HI I'm here")
            MESSAGE = "{},{},{}".format(key,value, timeStamp)
            print(MESSAGE)
            print('\n')

            # Pumping out the values
            sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
            # time.sleep(0.05)

            
async def get_position(drone):
    while True:
        async for position in drone.telemetry.position():
            altitude =  position.relative_altitude_m
            latitude =  position.latitude_deg
            longitude =  position.longitude_deg
            absolute_altitude = position.absolute_altitude_m
            break
        data = {               
            "latVal":latitude,
            "lngVal":longitude,
            "altVal":altitude,
            "A_altitude":absolute_altitude

        }
        timeStamp = time.time()
   
        for key,value in data.items():
            # print("HI I'm here")
            MESSAGE = "{},{},{}".format(key,value, timeStamp)
            print(MESSAGE)
            print('\n')

            # Pumping out the values
            sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
            # time.sleep(0.01)

    
    
async def get_battery(drone):
    while True:
        async for battery in drone.telemetry.battery():
            battery_remain =  battery.remaining_percent
            battery_volt =  battery.voltage_v
            break
        
        data = {               
            "battery_voltageVal":battery_volt,
            "battery_remainingVal":battery_remain
        }

        timeStamp = time.time()
        for key,value in data.items():
            MESSAGE = "{},{},{}".format(key,value, timeStamp)
            print(MESSAGE)
            print('\n')

            # Pumping out the values
            sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
            # time.sleep(0.01)


async def get_speed(drone):
    while True:
        async for fixedwing in drone.telemetry.fixedwing_metrics():
                airspeed = fixedwing.airspeed_m_s
                throttle = fixedwing.throttle_percentage
                climb_rate = fixedwing.climb_rate_m_s
                break
        data = {               
            "airspeedVal":airspeed,
            "throttle_percentage_Val":throttle,
            "climb_rate_Val":climb_rate

        }
        timeStamp = time.time()
        for key,value in data.items():
            # print("HI I'm here")
            MESSAGE = "{},{},{}".format(key,value, timeStamp)
            print(MESSAGE)
            print('\n')

            # Pumping out the values
            sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
            # time.sleep(0.05)

async def get_rawgps(drone):
    while True:
        async for r_gps in drone.telemetry.rawgps():
                G_velocity = r_gps.velocity_m_s
                break
        data = {               
            "velocity_Val":G_velocity

        }
        timeStamp = time.time()
        for key,value in data.items():
            # print("HI I'm here")
            MESSAGE = "{},{},{}".format(key,value, timeStamp)
            print(MESSAGE)
            print('\n')

            # Pumping out the values
            sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
            # time.sleep(0.05)

async def get_Pressure(drone):
    while True:
        async for scaled_preesure in drone.telemetry.ScaledPressure():
                ab_pressure = scaled_preesure.absolute_pressure_hpa
                di_pre_deg = scaled_preesure.differential_pressure_temperature_deg
                break
        data = {               
            "absolute_pressure":ab_pressure,
            "differential_pressure_temperature_deg":di_pre_deg
        }
        timeStamp = time.time()
        for key,value in data.items():
            # print("HI I'm here")
            MESSAGE = "{},{},{}".format(key,value, timeStamp)
            print(MESSAGE)
            print('\n')

            # Pumping out the values
            sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
            # time.sleep(0.05)

if __name__ == "__main__":
    # Start the main function
    asyncio.ensure_future(run())

    # Runs the event loop until the program is canceled with e.g. CTRL-C
    asyncio.get_event_loop().run_forever()

