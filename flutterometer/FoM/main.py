#     Copyright
#     (c) 2020                          _ /|_
#     German Aerospace Center (DLR)    /_/_/_/
#     Institute of Aeroelasticity        |/

# FLIPASED Flutterometer

# get data from mpu6050.py
# build data buffer
# pass data to modal analysis (SSI)
# pass data to telemetry

import numpy as np
from dataBuffer import dataBuffer
from multi_threading import multi_threading



import threading, time
import time
#import threading, queue

domains_time = []
domains_Ax = []
domains_Ay = []
domains_Az = []

start_time = time.time()
modal_params = []

# FIFO buffer
buffer_length = 100
# SSI update rate
update_rate = 4
# SSI parameters
blockSize = 8
modelOrder = 20
fs = 196
flight = 1

while True:
	for i in range(buffer_length):
	
		# build FIFO buffer
		domains_time, domains_Ax, domains_Ay, domains_Az = dataBuffer(domains_time, domains_Ax, domains_Ay, domains_Az, buffer_length)
	
		# modal analysis every 4 s
		diff_time = time.time()- start_time
		if diff_time > update_rate:

			# multi-threading
			thread_modal = threading.Thread(target=multi_threading, args=(domains_Ax, domains_Ay, domains_Az, blockSize, modelOrder, fs, flight))
			thread_modal.start()
			start_time = time.time()
		
			# reset buffer
			domains_time = []
			domains_Ax = []
			domains_Ay = []
			domains_Az = []
