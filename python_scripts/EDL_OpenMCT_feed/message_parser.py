import numpy as np
import struct

imu = [
    "err.Flag", 0
]

shm = [
    "err.Flag", 0
]


def parser(CommandMessage, MsgID, payload):
    #print(str(msg)+'imported!')
    # define the function blocks

## ErrorFlag
    errFlag = [
    "err.Flag", 0
    ]

    def errorFlag():
        return(errFlag)



## BATTERY
    bat = [
        "Bat.RX_MUX1", 0,
        "Bat.RX_MUX2", 0,
        "Bat.RX_MUX3", 0,
        "Bat.FlightHead", 0
    ]

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


## XSENS
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


## ADS
    ads = [
            'ads.altitude',        np.float32(0),
            'ads.angleOfAttack',      np.float32(0), 
            'ads.sideslip',      np.float32(0), 
            'ads.velocity', np.uint32(0)
        ]               

    def ADS():
        ads[1] = np.ndarray((1,), buffer = payload[0:3], dtype=np.dtype('>f4')) #alt
        ads[3] = np.ndarray((1,), buffer = payload[3:6], dtype=np.dtype('>f4')) #AoA
        ads[5] = np.ndarray((1,), buffer = payload[6:9], dtype=np.dtype('>f4')) #sideslip
        ads[7] = np.ndarray((1,), buffer = payload[9:12], dtype=np.dtype('>u4')) #v

        return(ads) 
    

## PPM
    ppm = [None] * 34
    count = 0
    for i in range(int(len(ppm)/2)):
        ppm[count] = 'ppm.ch'+str(i)
        count = count + 2
    
    def PPM():
        data = np.ndarray((16,1), buffer = payload, dtype=np.dtype('>u2')) #v
        count = 1
        for i in range(int(len(ppm)/2)):
            ppm[count] = data[i]
        return(ppm) 
## DD
    dd = [
    "dd.1", 0
    ]
    def DD():
        return(dd)     



## ECU
    ecu = [
    'ecu.FuelFlowPerSec', 0, #1
    'ecu.PupmVoltage', 0, #3
    'ecu.RPMActual', 0, #5
    'ecu.RPMControl', 0, #7
    'ecu.Status', 0, #9
    'ecu.Temp', 0, #11
    'ecu.Throttle', 0 #13
    ]

    def ECU():
        ecuStr = payload[2:].decode("utf-8") #evtl auch nutzbar in message_parser
        ecuFuel = struct.unpack(np.double, payload[0])[0] * 16 + struct.unpack(np.double, payload[1])[0]

        count = 1
        for i in range(int(len(ecuStr)/2)):
            firstByte = ecuStr[count-1]
            ecuStr[count-1] = ecuStr[count]
            ecuStr[count] = firstByte
            count = count + 2

        strArray = ecuStr.split(",")
        dataCount = len(strArray)

        if dataCount < 2:
            return(ecu)
        
        if strArray[1]=='WRP':
            ecu[7] = float(strArray[2]) #RPM
        elif dataCount == 5:
            ecu[5] = float(strArray[0]) #RPM actual
            ecu[11] = float(strArray[1]) #Temp
            ecu[3] = float(strArray[2]) #PumpV
            ecu[9] = float(strArray[3]) #Status
            ecu[13] = float(strArray[4]) #Throttle

        if ecu[5] > 0:
            ecu[1] = 0.1911 * ecuFuel +0.0606
        
        return[ecu]


        
## SERVO_REF
    servo_ref = [None] * 34
    count = 0
    for i in range(int(len(servo_ref)/2)):
        servo_ref[count] = 'servo_rev.ID'+str(i)
        count = count + 2
    
    def SERVO_REF():

        data = np.ndarray((21,1), buffer = payload, dtype=np.dtype('>u2')) #v
        count = 1
        for i in range(int(len(servo_ref)/2)):
            servo_ref[count] = data[i]
        
        return(servo_ref) 


## IMU
    def IMU():
        return(imu) 


## SHM
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
                        