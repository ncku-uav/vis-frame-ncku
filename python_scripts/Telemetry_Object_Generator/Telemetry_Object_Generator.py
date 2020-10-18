import json
import os

## Input
telemetry_object_name = 'Aircraft_42'
number_of_telemetry_points = 5

generate_Dictionary = False
generate_OpenMCT_object = False
generate_server_object = True



## generate Telemetry Dictionary (metadata for the Telemetry)
if generate_Dictionary:
    #
    new_telemetry_disctionary = {"name": telemetry_object_name, "key": "pl", "measurements": []}
    os.mkdir('../../openmct/example/'+telemetry_object_name)
    with open('Example_Telemetry_Object/EXAMPLEdictionary.json') as dictionary_example:
        data = json.load(dictionary_example)
        for i in range(number_of_telemetry_points):
            data['measurements'][0]['name'] = telemetry_object_name + ' Telemetry Point ' + str(i)
            data['measurements'][0]['key'] = 'key.' + str(i)
            copy = data['measurements'][0].copy() #if not working with copy, all elements have the indice of the last one
            new_telemetry_disctionary['measurements'].append(copy)
            #print(new_telemetry_disctionary['measurements'][0]['name']+ ' Entry Created!')

    with open('../../openmct/example/'+telemetry_object_name+'/'+telemetry_object_name+'dictionary'+'.json', 'w') as outfile:
        json.dump(new_telemetry_disctionary, outfile, indent=4)


## generate Telemetry Object for OpenMCT
if generate_OpenMCT_object:
    new_telemetry_object = ''
    with open('Example_Telemetry_Object/EXAMPLE-plugin.js') as object_example:
        data = object_example.read()
        new_telemetry_object = data.replace('EXAMPLE', telemetry_object_name)

    with open('../../openmct/example/'+telemetry_object_name+'/'+telemetry_object_name+'-plugin'+'.js', 'w') as outfile:
        outfile.write(new_telemetry_object)

## generate Telemetry Server Object
if generate_server_object:
    new_server_object = ''
    key = {"key.0": 0}
    for i in range(number_of_telemetry_points):
        key['key.'+str(i)] = 0
    print(str(key))
    with open('Example_Telemetry_Object/Example-TelemetryServerObject.js') as object_example:
        data = object_example.read()
        new_server_object = data.replace('Example', telemetry_object_name)
        keyPosition = new_server_object.find('key')

        new_server_object = new_server_object[0:keyPosition-2]+str(key)+new_server_object[keyPosition+10:]
        

    with open('../../OpenMCT_Telemetry_Server/'+telemetry_object_name+'.js', 'w') as outfile:
        outfile.write(new_server_object)


    
