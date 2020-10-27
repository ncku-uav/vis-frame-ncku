import numpy as np
import struct

def initalizeParser(StatusFlags, Battery, XSens, ADS, PPM, DD, ECU, ServoRef, IMU_1, IMU_2, SHM_1, SHM_2):

    if StatusFlags[0] == '1': 
        global errFlag
        errFlag = [
            "err.Flag", 0
            ]
    if Battery[0] == '1': 
        global bat
        bat = [
            "Bat.RX_MUX1", 0,
            "Bat.RX_MUX2", 0,
            "Bat.RX_MUX3", 0,
            "Bat.FlightHead", 0
        ]

    if XSens[0] == '1': 
        global xsens
        xsens = [
                'xSens.Hour',        np.uint8(0), #1
                'xSens.Minute',      np.uint8(0), #3
                'xSens.Second',      np.uint8(0), #5
                'xSens.Millisecond',np.uint16(0), #7
                'xSens.EulerA',          np.float32(0), #9
                'xSens.EulerB',          np.float32(0),#11
                'xSens.EulerY',          np.float32(0), #13
                'xSens.ECEFX',          np.float32(0),#15
                'xSens.ECEFY',          np.float32(0),#17
                'xSens.ECEFZ',          np.float32(0), #19
                'xSens.AngularVa',      np.float32(0),#21
                'xSens.AngularVb',      np.float32(0),#23
                'xSens.AngularVy',      np.float32(0), #25
                'xSens.AccX',         np.float32(0),#27
                'xSens.AccY',         np.float32(0),#29
                'xSens.AccZ',         np.float32(0), #31
                'xSens.VelX',             np.float32(0), #33
                'xSens.VelY',             np.float32(0), #35
                'xSens.Sec',            np.double(0), #37
                'xSens.Latency',        np.double(0) #39       
        ]  

    if ADS[0] == '1': 
        global ads
        ads = [
                'ads.altitude',        np.uint32(0),
                'ads.angleOfAttack',      np.uint32(0), 
                'ads.sideslip',      np.uint32(0), 
                'ads.velocity', np.uint32(0)
            ]
    if PPM[0] == '1': 
        global ppm
        ppm = [None] * 32
        count = 0
        for i in range(int(len(ppm)/2)):
            ppm[count] = 'ppm.ch'+str(i)
            count = count + 2 

    if DD[0] == '1': 
        global dd
        dd = [
        "dd.1", 0
        ]

    if ECU[0] == '1': 
        global ecu
        ecu = [
    'ecu.FuelFlowPerSec', 0, #1
    'ecu.PupmVoltage', 0, #3
    'ecu.RPMActual', 0, #5
    'ecu.RPMControl', 0, #7
    'ecu.Status', 0, #9
    'ecu.Temp', 0, #11
    'ecu.Throttle', 0 #13
    ]

    if ServoRef[0] == '1': 
        global servo_ref
        servo_ref = [None] * 42
        count = 0
        for i in range(int(len(servo_ref)/2)):
            servo_ref[count] = 'servo_rev.ID'+str(i)
            count = count + 2
        

    global imu
    imu=[]
    if IMU_1[0] == '1':
        imu.extend(['imu.AnlAccZ', 0])
    if IMU_1[1] == '1':
        imu.extend(['imu.GyroX', 0])
    if IMU_1[2] == '1':
        imu.extend(['imu.GyroY', 0])
    if IMU_1[3] == '1':
        imu.extend(['imu.DigAccZ', 0])
    count=1
    for bit in IMU_1[4:]:
        if bit=='1':
            imu.extend(['imu.IMU'+str(count), 0])
        count = count +1
    count=5
    for bit in IMU_2:
        if bit=='1':
            imu.extend(['imu.IMU'+str(count), 0])
        count = count +1

    global shm
    shm = []
    if SHM_1[0] == '1':
        shm.extend(['shm.Pos', 0])
    if SHM_1[1] == '1':
        shm.extend(['shm.Temp', 0])
    count=14
    for bit in SHM_1[2:]:
        if bit=='1':
            shm.extend(['shm.SHM'+str(count), 0])
        count = count -1
    count=8
    for bit in SHM_2:
        if bit=='1':
            shm.extend(['shm.SHM'+str(count), 0])
        count = count -1
    #print(shm)
    


def parser(CommandMessage, MsgID, payload):
    #print(str(msg)+'imported!')
    # define the function blocks

## ErrorFlag
    def errorFlag():
        #print(payload)
        flightHatU16 = np.ndarray((1,), buffer = payload[0:2], dtype=np.uint16)
        
        rx_mux1U16 = np.ndarray((1,), buffer = payload[2:4], dtype=np.uint16)
        rx_mux1TimeU16 = np.ndarray((1,), buffer = payload[4:6], dtype=np.uint16)

        rx_mux2U16 = np.ndarray((1,), buffer = payload[6:8], dtype=np.uint16)
        rx_mux2TimeU16 = np.ndarray((1,), buffer = payload[8:10], dtype=np.uint16)

        rx_mux3U16 = np.ndarray((1,), buffer = payload[10:12], dtype=np.uint16)
        rx_mux3TimeU16 = np.ndarray((1,), buffer = payload[12:14], dtype=np.uint16)

        #print(rx_mux1)

        return(errFlag)



## BATTERY
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
    def XSENS():
        xsens[1] = payload[60] #h
        xsens[3] = payload[61] #min
        xsens[5] = payload[64] #s
        xsens[7] = np.ndarray((1,), buffer = payload[62:64], dtype = np.uint16)[0] #ms
        xsens[9:14:2] = np.ndarray((3,), buffer = payload[0:12], dtype=np.dtype('>f4')) #Euler
        xsens[15:20:2] = np.ndarray((3,), buffer = payload[12:24], dtype=np.dtype('>f4')) #Pos
        xsens[21:26:2] = np.ndarray((3,), buffer = payload[24:36], dtype=np.dtype('>f4')) #AngV
        xsens[27:32:2] = np.ndarray((3,), buffer = payload[36:48], dtype=np.dtype('>f4')) #Acc
        xsens[33:38:2] = np.ndarray((3,), buffer = payload[48:60], dtype=np.dtype('>f4')) #V
        xsens[37] = np.double(xsens[5]) + np.double(xsens[7]/1000) #x_sec
        xsens[39] = 0
        #print(payload)
        #print(xsens[29:34:2])
        return(xsens)


## ADS
    def ADS():
        ads[1] = np.ndarray((1,), buffer = payload[0:4], dtype=np.dtype('>f4'))[0] #alt
        ads[3] = np.ndarray((1,), buffer = payload[4:8], dtype=np.dtype('>f4'))[0] #AoA
        ads[5] = np.ndarray((1,), buffer = payload[8:12], dtype=np.dtype('>f4'))[0] #sideslip
        ads[7] = np.ndarray((1,), buffer = payload[12:16], dtype=np.dtype('>u4'))[0] #v
        return(ads) 
    

## PPM
    def PPM():
        data = np.ndarray((16,), buffer = payload, dtype=np.dtype('>u2')) #v
        count = 1
        for i in range(int(len(ppm)/2)-1):
            #print(i)
            ppm[count] = data[i]
            count = count + 2
        #print(ppm)
        
        return(ppm) 
## DD
    def DD():
        return(dd)     

## ECU
    def ECU():
        #print(payload)
        ecuStr = (payload[2:-2]).tostring().decode("ascii")#payload[2:].astype('b') #evtl auch nutzbar in message_parser
        ecuStrList = list(ecuStr)
        # print(ecuStr)
        # print "".join([chr(item) for item in (payload[2:-2])])
        ecuFuel = payload[0] * 16 + payload[1]

        count = 1
        for i in range(int(len(ecuStrList)/2)):
            firstByte = ecuStrList[count-1]
            ecuStrList[count-1] = (ecuStrList[count])
            ecuStrList[count] = str(firstByte)
            count = count + 2

        #print(ecuStrList)
        ecuStrRearranged = "".join([str(elem) for elem in ecuStrList])
        strArray = ecuStrRearranged.replace('\x00','').replace('\r','').split(',')
        #print(ecuStrRearranged)
        #print(strArray)
        dataCount = len(strArray)

        if dataCount < 2:
            return(ecu)
        
        if strArray[1]=='WRP':
            ecu[7] = strArray[2]#.encode('ascii', 'ignore').decode('unicode_escape') #RPM control
        elif dataCount == 5:
            ecu[5] = strArray[0] #RPM actual
            ecu[11] = strArray[1]
            ecu[3] = strArray[2]
            ecu[9] = strArray[3]
            ecu[13] = strArray[4]

        if ecu[5] > 0:
            ecu[1] = 0.1911 * ecuFuel +0.0606
        
        #print(ecu)
        
        return(ecu)

        
## SERVO_REF
    def SERVO_REF():

        data = np.ndarray((21,), buffer = payload, dtype=np.dtype('>u2')) #v
        count = 1
        for i in range(int(len(servo_ref)/2)-1):
            servo_ref[count] = data[i]
            count = count + 2
        #print(servo_ref)
        return(servo_ref) 


## IMU
    def IMU():
        #print(payload)
        return(imu) 


## SHM
    def SHM():
        #print(payload)
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
                        