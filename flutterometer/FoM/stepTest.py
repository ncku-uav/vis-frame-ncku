import numpy as np
from mpu6050 import goData
from dataBuffer import dataBuffer
from FLIPASED_SSI import *

#-----------------------------------------
# Step 1 - read mpu6050 data [WORKS]
#Ax, Ay, Az = goData()
#print(Ax, Ay, Az)

#---------------------------------------
# step 2 - dataBuffer FIFO
domains_time = []
domains_Ax = []
domains_Ay = []
domains_Az = []
buffer_length = 100

# this is too SLOW
for i in range(buffer_length):
	domains_time, domains_Ax, domains_Ay, domains_Az = dataBuffer(domains_time, domains_Ax, domains_Ay, domains_Az, buffer_length)

#print(domains_time,domains_Ax,domains_Ay,domains_Az)
#ax = list(domains_Ax)
#print(ax)
#dt = domains_time[1].microsecond - domains_time[0].microsecond
#print(dt)
#sample frequency 196 Hz

#----------------------------------------
# step 3 - modal analysis
time_vector = list(domains_time)
dAx = list(domains_Ax)
dAy = list(domains_Ay)
dAz = list(domains_Az)

# system matrix for
data = np.array([dAx, dAy, dAz])
s = data.shape
print(s)

# SSI
blockSize = 8
modelOrder = 20
fs = 196
flight = 1
modal_params = FLIPASED_SSI(data , blockSize, modelOrder, fs, flight)

f = modal_params.f
zeta = modal_params.zeta

print('Frequency')
print(f)
print('Damping')
print(zeta)

#-------------------------------------
# step 4 - build np array for telemetry

# build time vector
rows = f.shape
current_time = time.time()
time_vector = np.full((rows[0],),current_time)

# build ID vector
ID = np.arange(rows[0])

# create numpy array for telemetry
sys = np.array([time_vector, f, zeta, ID]).T

sshp = sys.shape
print(sshp)








#from dataBuffer import dataBuffer

#d_time = []
#d_Ax = []
#d_Ay = []
#d_Az = []
#buffer_length = 1000

#def stepTest():
#	for i in range(buffer_length):

#		d_time, d_Ax, d_Ay, d_Az = dataBuffer(d_time, d_Ax, d_Ay, d_Az, buffer_length)
		
	#return d_time, d_Ax, d_Ay, d_Az
#	print(d_time)
