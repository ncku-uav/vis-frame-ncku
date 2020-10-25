import numpy as np

errFlag = [
    "err.Flag", 0
]

ads = [
    "err.Flag", 0
]

ppm = [
    "err.Flag", 0
]

dd = [
    "err.Flag", 0
]

ecu = [
    "err.Flag", 0
]

servo_ref = [
    "err.Flag", 0
]

imu = [
    "err.Flag", 0
]

shm = [
    "err.Flag", 0
]

bat = [
    "Bat.RX_MUX1", 0,
    "Bat.RX_MUX2", 0,
    "Bat.RX_MUX3", 0,
    "Bat.FlightHead", 0
]
def parser(configID, MsgID, payload):
    #print(str(msg)+'imported!')
    # define the function blocks
    def errorFlag():
        return(errFlag)

    def Battery():
        #bat = np.zeros(4)
        size_t = 2
        count=1
        for i in range(4):
            beginIndex = i*size_t
            endIndex= beginIndex+size_t
            tempData = np.ndarray((1,), buffer = payload[beginIndex:endIndex], dtype=np.uint16)
            if tempData.size == 1:
                bat[count] = tempData[0]
            count = count+2
            #print(bat)
        return(bat)

    xsens = [
            'xSens.Hour',        np.uint8(0),
            'xSens.Minute',      np.uint8(0), 
            'xSens.Second',      np.uint8(0), 
            'xSens.Millisecond',np.uint16(0), 
            'xSens.EulerA',          np.float32(0),
            'xSens.EulerB',          np.float32(0),
            'xSens.EulerY',          np.float32(0), 
            'xSens.ECEFX',          np.float32(0),
            'xSens.ECEFY',          np.float32(0),
            'xSens.ECEFZ',          np.float32(0), 
            'xSens.AngularVa',      np.float32(0),
            'xSens.AngularVb',      np.float32(0),
            'xSens.AngularVy',      np.float32(0), 
            'xSens.AccX',         np.float32(0),
            'xSens.AccY',         np.float32(0),
            'xSens.AccZ',         np.float32(0), 
            'xSens.VelX',             np.float32(0),
            'xSens.VelX',             np.float32(0), 
            'xSens.Sec',            np.double(0), 
            'xSens.Latency',        np.double(0)        
        ]   
    #print(xsens[9:14])    

    def XSENS():
        xsens[1] = payload[60] #h
        xsens[3] = payload[61] #min
        xsens[5] = payload[64] #s
        xsens[7] = np.ndarray((1,), buffer = payload[62:64], dtype = np.uint16) #ms
        xsens[9:14:2] = np.ndarray((3,1), buffer = payload[0:12], dtype=np.dtype('>f4')) #Euler
        xsens[14:19:2] = np.ndarray((3,1), buffer = payload[12:24], dtype=np.dtype('>f4')) #Pos
        xsens[19:24:2] = np.ndarray((3,1), buffer = payload[24:36], dtype=np.dtype('>f4')) #AngV
        xsens[24:29:2] = np.ndarray((3,1), buffer = payload[36:48], dtype=np.dtype('>f4')) #Acc
        xsens[29:34:2] = np.ndarray((3,1), buffer = payload[48:60], dtype=np.dtype('>f4')) #V
        xsens[19] = np.double(xsens[5]) + np.double(xsens[7]/1000) #x_sec
        xsens[21] = 0
        #print(payload)
        #print(xsens[29:34:2])
        return(xsens)

    def ADS():
        return(ads) 

    def PPM():
        return(ppm) 

    def DD():
        return(dd) 
    
    def ECU():
        return(ecu) 
    
    def SERVO_REF():
        return(servo_ref) 

    def IMU():
        return(imu) 
    
    def SHM():
        return(shm) 

    # map the inputs to the function blocks
    options = {0 : errorFlag,
            7 : Battery,
            14: XSENS,
            21: ADS, 
            28: PPM,
            35: DD,
            42: ECU,
            49: SERVO_REF,
            56: IMU,
            63: SHM
    }
    
    return(options[MsgID]())
                        