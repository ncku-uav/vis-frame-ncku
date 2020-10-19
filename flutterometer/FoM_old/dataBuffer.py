# build data buffer
import numpy as np
#from mpu6050 import goData
from datetime import datetime
from collections import deque
import random
import time

def dataBuffer(domains_time, domains_Ax, domains_Ay, domains_Az, buffer_length):

	if len(domains_time) == 0:
		
		#buffer_length = 1000
		domains_time = np.zeros(buffer_length)
		domains_Ax = np.zeros(buffer_length)
		domains_Ay = np.zeros(buffer_length)
		domains_Az = np.zeros(buffer_length)

	
	d_time = deque(domains_time)
	d_Ax = deque(domains_Ax)
	d_Ay = deque(domains_Ay)
	d_Az = deque(domains_Az)

	d_time.pop()
	d_Ax.pop()
	d_Ay.pop()
	d_Az.pop()

	try:
		from mpu6050 import goData
		Ax, Ay, Az = goData()
	except:
		#print("No sensor connected")
		Ax = random.randint(0,10)
		Ay = random.randint(0,10)
		Az = random.randint(0,10)
		time.sleep(0.01)
	
	now = datetime.now()
	#current_time = now.strftime("%Y:%m:%d:%H:%M:%S")

	d_time.appendleft(now)
	d_Ax.appendleft(Ax)
	d_Ay.appendleft(Ay)
	d_Az.appendleft(Az)       

	return d_time, d_Ax, d_Ay, d_Az
