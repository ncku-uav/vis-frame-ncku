# assists in creating new implementations of OpenMCT
# can generate: a Dictionary, a OpenMCT Plugin, the corresponding openmct server object
# after creating/generating the objects, add them (in the style of Aircraft 42) to OpenMCT_Telemetry_Server/server.js and openmct/index.html

import json
import os

## Input
telemetry_object_name = 'Aircraft_42'
number_of_telemetry_points = 5 # can be altered afterwards
UDP_PORT = '50015' # choose a different UDP for every implementation


generate_Dictionary = True
generate_OpenMCT_object = True
generate_server_object = True
generate_pythonScript = True



## generate Telemetry Dictionary (metadata for the Telemetry)
if generate_Dictionary:

    #Initialisation
    new_telemetry_disctionary = {"name": telemetry_object_name, "key": "pl", "measurements": []}
    os.mkdir('../../openmct/example/'+telemetry_object_name)

    # Open Example and save a modified copy
    with open('Example_Telemetry_Object/EXAMPLEdictionary.json') as dictionary_example:
        data = json.load(dictionary_example)
        for i in range(number_of_telemetry_points):
            data['measurements'][0]['name'] = telemetry_object_name + ' Telemetry Point ' + str(i)
            data['measurements'][0]['key'] = 'key.' + str(i)
            copy = data['measurements'][0].copy() #if not working with copy, all elements have the indice of the last one
            new_telemetry_disctionary['measurements'].append(copy)
            #print(new_telemetry_disctionary['measurements'][0]['name']+ ' Entry Created!')

    # Save modified copy to the right Folder
    with open('../../openmct/example/'+telemetry_object_name+'/'+telemetry_object_name+'dictionary'+'.json', 'w') as outfile:
        json.dump(new_telemetry_disctionary, outfile, indent=4)



## generate Telemetry Object for OpenMCT
if generate_OpenMCT_object:

    #Initialisation
    new_telemetry_object = ''

    # Open Example and save a modified copy
    with open('Example_Telemetry_Object/EXAMPLE-plugin.js') as object_example:
        data = object_example.read()
        new_telemetry_object = data.replace('EXAMPLE', telemetry_object_name)

    # Save modified copy to the right Folder
    with open('../../openmct/example/'+telemetry_object_name+'/'+telemetry_object_name+'-plugin'+'.js', 'w') as outfile:
        outfile.write(new_telemetry_object)



## generate Telemetry Server Object
if generate_server_object:

    #Initialisation
    new_server_object = ''
    key = {"key.0": 0}
    for i in range(number_of_telemetry_points): #generate consistent keys for the server object
        key['key.'+str(i)] = 0
    
    # Open Example and save a modified copy
    with open('Example_Telemetry_Object/Example-TelemetryServerObject.js') as object_example:
        data = object_example.read()
        new_server_object = data.replace('EXAMPLE', telemetry_object_name)
        new_server_object = new_server_object.replace('server.bind()', 'server.bind('+UDP_PORT+')')
        # keyPosition = new_server_object.find('key')
        # new_server_object = new_server_object[0:keyPosition-2]+str(key)+new_server_object[keyPosition+10:]
        
    # Save modified copy to the right Folder
    with open('../../OpenMCT_Telemetry_Server/'+telemetry_object_name+'.js', 'w') as outfile:
        outfile.write(new_server_object)


## generate Python Script
if generate_pythonScript:

    #Initialisation
    keys = ''
    # Open Example and save a modified copy
    with open('Example_Telemetry_Object/EXAMPLE-python-script.py') as python_example:
        data = python_example.read()

       
        for i in range(number_of_telemetry_points): #generate consistent keys
            keys = keys + '"key.'+str(i)+'",'
            

        new_python_script = data.replace('EXAMPLE', telemetry_object_name)
        new_python_script = new_python_script.replace('herekeys', keys)
        new_python_script = new_python_script.replace('UDP_PORT =', 'UDP_PORT =' + UDP_PORT)

            
    # Save modified copy to the right Folder
    with open('../'+telemetry_object_name+'_OpenMCT_feed_artificial'+'.py', 'w') as outfile:
        outfile.write(new_python_script)


    
