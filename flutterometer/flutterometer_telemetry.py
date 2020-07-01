import numpy as np
import matplotlib.pyplot as plt
import time
from datetime import datetime

class flutterometer_telemetry:
    
    def __init__(self, count):
        self.sys = self.build_sys_matrix(count)
        
    def build_sys_matrix(self, count):
        #count = 0
        sys = []
        mat = []
        id1 =1
        id2 = 6
        id3 = 14

        f1 = np.sin(count)
        f2 = np.sin(1.5*count)+3
        f3 = np.sin(0.5*count)+6
        d1 = 0.2*np.sin(3*count)
        d2 = 0.3*np.sin(5*count)+3
        d3 = 0.4*np.sin(7*count)+6
        now = datetime.now()
        current_time = now.strftime("%Y:%m:%d:%H:%M:%S")
        sys = np.array([[current_time,f1,d1,id1],[current_time,f2,d2,id2],[current_time,f3,d3,id3]])       
        time.sleep(2)
        return sys




