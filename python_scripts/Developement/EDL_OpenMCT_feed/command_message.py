import numpy as np

def getCommandMsg():
    
    configId = 12 #configuration identifier for flight head

    header = np.array([0,0], dtype=np.uint8)
    commandMessage = np.array(5, dtype=np.uint8)
    footer = 0#np.uint8(1)
    message = np.zeros(8, dtype=np.uint8)


    head_fix = '96'
    header[0] = (np.uint8(int(head_fix, 16)))
    header[1] = (np.uint8(configId))
    #print(header)
    
    commandMessage = [251,255,255,255,255]

    message[0:2] = header
    message[2:7] = commandMessage
    
    for part in message[0:7]:
        footer = np.uint8(np.bitwise_xor(footer,part))
        
    print(footer)
    
    message[7] = footer

    return(message)
