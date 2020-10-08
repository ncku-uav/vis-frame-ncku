//https://hackernoon.com/arduino-serial-data-796c4f7d27ce


var SerialPort = require('serialport');

var serialPort = /*new*/ SerialPort('/dev/ttyUSB0', {
    baudRate: 9600,
    //parser: SerialPort.parsers.Readline("\n")
});


// Switches the port into "flowing mode"
serialPort.on('data', function (data) {
    console.log('Data:', data);
});

// Read data that is available but keep the stream from entering //"flowing mode"
serialPort.on('readable', function () {
    console.log('Data:', port.read());
});
