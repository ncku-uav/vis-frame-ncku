// telemetry source object for the Aircraft_42

const dgram = require('dgram');
const server = dgram.createSocket('udp4');
const fs = require('fs');


function Aircraft_42() {

	
	// Initialize working Parameters and Object

	// read the {'key.0': 0, 'key.1': 0, 'key.2': 0, 'key.3': 0, 'key.4': 0} dictionary
	let rawDict = fs.readFileSync('../openmct/example/Aircraft_42/Aircraft_42dictionary.json')
	let dict = JSON.parse(rawDict)
	//console.log(dict.measurements.map(obj => obj.key))

	this.state={};
	(dict.measurements.map(obj => obj.key)).forEach(function (k) {
		this.state[k] = 0;
	}, this);
	//console.log(this.state)

    this.history = {};
    this.listeners = [];
	this.data = [];
	Object.keys(this.state).forEach(function (k) {
        this.history[k] = [];
	}, this);

	// to notify telemetry server interval based (STFE) uncomment here
    setInterval(function () {
        this.generateIntervalTelemetry();
    }.bind(this), 100); //z.B. 100ms according to SFTE

    var count = 0
	var initGPSheight = 0;
	//what to do, when a message from the UDP Port arrives
    server.on('message', (msg, rinfo) => {
		//parse the data
		this.data = `${msg}`.split(',');
		
		// Check server message
		//console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`)
        //console.log(`server got: ${this.data[8]} from ${rinfo.address}:${rinfo.port}`)
		
		//Save the data to the state array
		this.state[this.data[0]] = this.data[1];
		this.state['Time.stamp'] = Math.round(this.data[2]*1000); //convert python timestamp[s] to JS timestamp [ms]
		
		//console.log(Date.now()-this.state['Time.stamp']) //check lag incomming
		//console.log(this.state['Time.stamp']) //check Timestamp
		//console.log(this.state[this.data[0]]) //check paylaod
		
		//CALCULATIONS

			// e.g. change gps from mm to m
			// if (this.data[0] === 'data.gps.heightMS' || this.data[0] === 'data.gps.gSpeed'){
			// this.state[this.data[0]] = Math.round((this.data[1]/1000 - this.initGPSheight) *100)/100;
			// }
			// if (this.data[0] === 'data.gps.gSpeed'){
			// this.state[this.data[0]] = this.data[1]/1000;
			// }

			
		//console.log(this.state);
		

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

	});
	server.on('error', (err) => {
		console.log(`Aircraft_42 UDP server error:\n${err.stack}`);
		server.close();
	  });

	// port specified in the associated python scrip
	server.bind(50015);

    console.log("Aircraft_42 initialized!");
};


// to update every time new data comes in
Aircraft_42.prototype.generateRealtimeTelemetry = function () {

	// Real Timestamp
	var timestamp = this.state['Time.stamp'];
	// Artificial timestamp
	//var timestamp= Date.now();

	// built message
	var message = { timestamp: timestamp, value: this.data[1], id: this.data[0]}; //for real time telemetry id is needed
	// notify realtimeserver
	this.notify(message);
	//console.log(message);

}


// to update interval based (STFE)
Aircraft_42.prototype.generateIntervalTelemetry = function () {

    Object.keys(this.state).forEach(function (id) {

        // Real Timestamp
		var timestamp = this.state['Time.stamp'];
		// Artificial timestamp
        //var timestamp= Date.now();

		// built message
		var message = { timestamp: timestamp, value: this.state[id], id: id}; //for real time telemetry id is needed
		// notify realtimeserver
        this.notify(message);
		
	}, this);
	//console.log(state);
};


// notifiy function, called in generate Telemetry, notifies listeners
Aircraft_42.prototype.notify = function (point) {
    this.listeners.forEach(function (l) {
        l(point);
    });
};


// manages listeners for realtime telemetry
Aircraft_42.prototype.listen = function (listener) {
    this.listeners.push(listener);
    return function () {
        this.listeners = this.listeners.filter(function (l) {
            return l !== listener;
        });
    }.bind(this);
};



// what to do on incoming command
Aircraft_42.prototype.command = function (command) {

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
			fs.writeFile(__dirname + '/saved_logs/Aircraft_42_'+seedatetime+'.json', write, (err) => {
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
				fs.writeFile(__dirname + '/saved_logs/Aircraft_42_'+seedatetime+'.json', write, (err) => {
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
    return new Aircraft_42()
};
