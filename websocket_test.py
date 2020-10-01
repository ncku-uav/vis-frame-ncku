import websocket
import socket
import json


try:
    import thread
except ImportError:
    import _thread as thread
import time

UDP_IP = "127.0.0.1"
UDP_PORT = 50013
MESSAGE = "23,567,32,4356,456,132,4353467"

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # Internet, UDP
try:
    sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
except:
    print('Initial message failed!')

roll= pitch = yaw = nav_roll = nav_pitch= groundcourse= alt = satcount= airspeed= tar_airspeed=groundspeed=wind_dir= wind_vel= ax= ay= az= mx= my= mz= battV= lidar_alt=radius=climbrate= turnrate= distTraveled = 1



def on_message(ws, message):
    #print(message)
    jsonParse = json.loads(message)
    roll= pitch = yaw = nav_roll = nav_pitch= groundcourse= alt = satcount= airspeed= tar_airspeed=groundspeed=wind_dir= wind_vel= ax= ay= az= mx= my= mz= battV= lidar_alt=radius=climbrate= turnrate= distTraveled = jsonParse['MQTT']['GPS']['alt']
    
    # Build a message, add new curly bracket and variable name
    MESSAGE = "{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}".format(roll, pitch, yaw, nav_roll, nav_pitch, groundcourse, alt, satcount, airspeed, tar_airspeed,groundspeed,wind_dir, wind_vel, ax, ay, az, mx, my, mz, battV, lidar_alt,radius,climbrate, turnrate, distTraveled)
    # Show the timestep
    print(MESSAGE)
    #print(type(MESSAGE))
    print('\n')

    # Pumping out the values
    sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))

def on_error(ws, error):
    print(error)

def on_close(ws):
    print("### closed ###")

def on_open(ws):
    print("open")


if __name__ == "__main__":
    websocket.enableTrace(True)
    ws = websocket.WebSocketApp("ws://192.168.0.110:443",
                              on_message = on_message,
                              on_error = on_error,
                              on_close = on_close)
    ws.on_open = on_open
    ws.run_forever()