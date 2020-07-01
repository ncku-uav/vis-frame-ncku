from flutterometer_telemetry import *

count = 0
while count < 100:
    sys = flutterometer_telemetry(count)
    print("New data received!")
    print(sys.sys)
    count += 1