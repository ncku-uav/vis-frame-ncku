// telemetry source object for the FLIPASED implementation

const dgram = require('dgram');
const server = dgram.createSocket('udp4');


function Flutterometer() {
    this.numberIds = 20;
    this.idString = 0;
    this.state = {}
    this.timestamp = Date.now();
    
    // Initialize working Parameters and Object

	// read the keys from dictionary
	let rawDict = fs.readFileSync('../openmct/example/DG800/DG800dictionary.json')
	let dict = JSON.parse(rawDict)
	//console.log(dict.measurements.map(obj => obj.key))

    // old initialisation of keys, new one not yet testet with live telemetry
    // for (var i = 0; i <= this.numberIds; ++i){
    //     this.state[`id${i}.Time`] = Date.now();
    //     this.state[`id${i}.Freq`] = 0;
    //     this.state[`id${i}.Damp`] = 0;
        
    // }
    //console.log(this.state['id3.Time'])

    this.state={};
	(dict.measurements.map(obj => obj.key)).forEach(function (k) {
		this.state[k] = 0;
	}, this);
	//console.log(this.state)

    this.history = {};
    this.listeners = [];
    this.data = new Array();
    Object.keys(this.state).forEach(function (k) {
        this.history[k] = [];
    }, this);

    // to notify telemetry server interval based (STFE) uncomment here
    setInterval(function () {
        this.generateTelemetry();
    }.bind(this), 100); //z.B. 100ms according to SFTE

    this.count = 0
    //what to do, when a message from the UDP Port arrives
    server.on('message', (msg, rinfo) => {
        this.data = `${msg}`.split(',');
		console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`)
        
        if(this.data[0] != 'nan') {
        
        // update current state
        this.idString = "id" + Math.round(this.data[0]).toString();
        console.log(this.idString)
        this.state[this.idString + ".Freq"] = this.data[1];
        this.state[this.idString + ".Damp"] = this.data[2];
        this.state[this.idString + ".Time"] = this.data[3];

        
		//console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`)
        //console.log(`server got: ${this.data[8]} from ${rinfo.address}:${rinfo.port}`)
        
        // old implementation without keys
        // this.count = 0;
        // for (var i = 1; i <= this.numberIds; ++i){
        //     this.state[`id${i}.Time`] = this.data[this.count];
        //     this.state[`id${i}.Freq`] = this.data[this.count+1];
        //     this.state[`id${i}.Damp`] = this.data[this.count+2];
        //     this.count = this.count + 4;
        //console.log(this.state[this.idString +".Time"])
            
        // }
        //// to notify telemetry server every time new data arrives in uncomment here
		//this.generateRealtimeTelemetry();


		//// Save History on every message, for high resolution
		// Real Timestamp
		var timestamp = this.state['Time.stamp'];
		// Artificial timestamp (if no timestamp is sent)
		//var timestamp= Date.now();

		// built message
		var message = { timestamp: timestamp, value: this.state[this.data[0]]};//, id: this.data[0]};
			try{ // store in history
				this.history[this.data[0]].push(message);
				//console.log(this.history[this.data[0]])
				//JSON.stringify(this.history[this.data[0]])
				}
				catch (e) {
					console.log(e)
                }
                
        }

    });
    server.on('error', (err) => {
		console.log(`DG800 UDP server error:\n${err.stack}`);
		server.close();
      });
      
    // port specified in the associated python scrip
	server.bind(50013);

    console.log("Flutter-o-Meter initialized!");

};




// to update interval based (STFE)
Flutterometer.prototype.generateTelemetry = function () {

    Object.keys(this.state).forEach(function (id) {
        
        if (id.slice(4,8) === 'Time'){
			// Actual Timestamp from received data
            this.timestamp = Math.round(this.state[id]*1000);
            //console.log(this.timestamp)
			// Artificial timestamp
            // this.timestamp= Date.now();
        }

       	// built message
		var message = { timestamp: timestamp, value: this.state[id], id: id}; //for real time telemetry id is needed
		// notify realtimeserver
        this.notify(message);
        
    }, this);
    
};

// notifiy function, called in generate Telemetry, notifies listeners

Flutterometer.prototype.notify = function (point) {
    this.listeners.forEach(function (l) {
        l(point);
    });
};

// manages listeners for realtime telemetry
Flutterometer.prototype.listen = function (listener) {
    this.listeners.push(listener);
    return function () {
        this.listeners = this.listeners.filter(function (l) {
            return l !== listener;
        });
    }.bind(this);
};

// what to do on incoming command
Flutterometer.prototype.command = function (command) {

	if(command === ':saveHistory'){
		//zero needed for right time and date format when copy-pasting in OpenMCT
		addZero = function(dateNumber) {
			if (dateNumber.toString().length == 1){
				dateNumber = '0'+dateNumber.toString()
			}
			return dateNumber
		}
		
		//Generate timestamp for the File
		var date = new Date();
		var year = date.getFullYear();
		var month = addZero(date.getMonth() + 1);      // "+ 1" because the 1st month is 0
		var day = addZero(date.getDate());
		var hour = addZero(date.getHours());
		var minutes = addZero(date.getMinutes());
		var seconds = addZero(date.getSeconds());
		var seedatetime = year+ '-'+ month+ '-'+ day+ ' '+ hour+ '-'+ minutes+ '-'+ seconds;

		//Using Promises for not interrupting the main loop
		function asyncSaveHistory(str) {
			return new Promise((resolve, reject) => {
			  resolve(JSON.stringify(str));
			});
		  }

		asyncSaveHistory(this.history).then(function(write) {//write is the value of the resolved promise in asyncStringify
			fs.writeFile(__dirname + '/saved_logs/FLIPASED_'+seedatetime+'.json', write, (err) => {
				if (err) {
					throw err;
				}
				console.log('History Saved!')
			}) 
		});
	
	};

	if(command === ':startLog'){
		//zero needed for right time and date format when copy-pasting in OpenMCT
		addZero = function(dateNumber) {
			if (dateNumber.toString().length == 1){
				dateNumber = '0'+dateNumber.toString()
			}
			return dateNumber
		}
		
		//Generate timestamp for the File
		var date = new Date();
		var year = date.getFullYear();
		var month = addZero(date.getMonth() + 1);      // "+ 1" because the 1st month is 0
		var day = addZero(date.getDate());
		var hour = addZero(date.getHours());
		var minutes = addZero(date.getMinutes());
		var seconds = addZero(date.getSeconds());
		var seedatetime = year+ '-'+ month+ '-'+ day+ ' '+ hour+ '-'+ minutes+ '-'+ seconds;

		//Using Promises for not interrupting the main loop
		function asyncLogging(src) {
			return new Promise((resolve, reject) => {
				
			  resolve(JSON.stringify(src));
			});
		  }
		
	
		// save log in specified interval  
		logging = setInterval(function () {
			asyncLogging(this.history).then(function(write) {//write is the value of the resolved promise in asyncStringify
				fs.writeFile(__dirname + '/saved_logs/FLIPASED_'+seedatetime+'.json', write, (err) => {
					if (err) {
						throw err;
					}
					//console.log(this.history);
					console.log('Logging!')
				}) 
			}.bind(this));
		}.bind(this), 10000); //z.B. 100ms according to SFTE
	
	};

	if(command === ':endLog'){
		clearInterval(logging);
		console.log('Logging stopped!')	
	};

	if(command === ':exampleCommandtoPlane'){
		// sending to the udp port 60012 on the address 'loacalhost'
		server.send(command,60012, 'localhost')
	};

	
};

module.exports = function () {
    return new Flutterometer()
};
