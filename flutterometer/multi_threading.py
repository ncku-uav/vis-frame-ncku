import numpy as np
import threading, time
import threading, queue
from FLIPASED_SSI import *
from tel_mod import tel_mod
import time

def multi_threading(d_Ax, d_Ay, d_Az, blockSize, modelOrder, fs, flight):
	
	# convert to array
	dAx = list(d_Ax)
	dAy = list(d_Ay)
	dAz = list(d_Az)

	# system matrix for SSI
	data = np.array([dAx, dAy, dAz])
	modal_params = FLIPASED_SSI(data , blockSize, modelOrder, fs, flight)
	rows = modal_params.f.shape

	# build time vector
	current_time = time.time()
	time_vector = np.full((rows[0],),current_time)

	# build ID vector
	ID = np.arange(rows[0])

	# create numpy array for telemetry
	sys = np.array([time_vector, modal_params.f, modal_params.zeta, ID]).T
	sys_shape = sys.shape
	#print(sys_shape)
	#print(modal_params.f)

	# build consistent matrix size
	tel_stream = np.empty((20,4))
	tel_stream[:] = np.nan
	tel_stream[0:sys_shape[0],0:sys_shape[1]] = sys
	tel_shape = tel_stream.shape
	print(tel_shape)
	print(tel_stream)
	
	# send through Telemetry when ready
	for x in range(9):
		tel_mod(tel_stream)
		time.sleep(0.1)
	#print(modal_params)


