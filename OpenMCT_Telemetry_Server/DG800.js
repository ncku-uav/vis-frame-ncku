
//const ntpsync = require('ntpsync');
const dgram = require('dgram');
const server = dgram.createSocket('udp4');
const fs = require('fs');


function Dg800() {

	
	// Initialize working Parameters and Object

	// read the keys from dictionary
		let rawDict = fs.readFileSync('../openmct/example/DG800/DG800dictionary.json')
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
    // setInterval(function () {
    //     this.generateTelemetryInterval();
    // }.bind(this), 100); //z.B. 100ms

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

			// change gps from mm to m
			if (this.data[0] === 'data.gps.heightMS' || this.data[0] === 'data.gps.gSpeed'){
			this.state[this.data[0]] = Math.round((this.data[1]/1000 - this.initGPSheight) *100)/100;
			}
			if (this.data[0] === 'data.gps.gSpeed'){
			this.state[this.data[0]] = this.data[1]/1000;
			}

			// rescale thrust ppm for display
			if (this.data[0] === 'data.nano.ch1'){
			this.state[this.data[0]] = this.data[1]/40;
			}


			// initial ground height
			if (this.data[0] === 'data.gps.heightMS' && this.state['data.gps.fixType'] === '3' && count < 100){
			initGPSheight = this.data[1]/1000;
			count = count + 1;
			}

/*			// fuel consumption in percent
			if (this.data[0] === 'data.nano.v'){
			this.state['data.nano.v'] = this.data[1]/7.5
			console.log(this.state[this.data[0]])
			}
*/
			// calculation airspeed
			if (this.data[0] === 'data.adp.pdyn'){
			this.state['data.adp.AirSpeed'] = Math.round((Math.sqrt((2*Math.abs(this.state['data.adp.pdyn']))/(this.state['data.adp.pstat']/(287*(parseFloat(this.state['data.thr.temp1'])+273.15)))))*100)/100;
			//console.log(this.state['data.adp.AirSpeed']);
			}

			// round Fuel Mass Flow
			if (this.data[0] === 'data.nano.v'){
			this.state['data.nano.v'] = Math.round(this.data[1]/7.5*100)/100;
			//console.log(this.state['data.adp.AirSpeed']);
			}

		//console.log(this.state);
		
		// to notify telemetry server every time new data arrives in uncomment here
		this.generateTelemetry();

		// SAVE HISTORY HERE?! or downsample when called on interval?
		// // Real Timestamp
		// var timestamp = this.state['Time.stamp'];
		// // Artificial timestamp
		// //var timestamp= Date.now();

		// // built message
		// var message = { timestamp: timestamp, value: this.data[1], id: this.data[0]};
		// 	try{ // store in history
		// 		this.history[this.data[0]].push(message);
		// 		}
		// 		catch (e) {
		// 			console.log(e)
		// 		}

	});
	server.on('error', (err) => {
		console.log(`DG800 UDP server error:\n${err.stack}`);
		server.close();
	  });

	// port specified in the associated python scrip
	server.bind(50012);

    console.log("DG-800 initialized!");
};


// to update every time new data comes in
Dg800.prototype.generateTelemetry = function () {

	// Real Timestamp
	var timestamp = this.state['Time.stamp'];
	// Artificial timestamp
	//var timestamp= Date.now();

	// built message
	var message = { timestamp: timestamp, value: this.data[1], id: this.data[0]};
	// notify realtimeserver
	this.notify(message);
	//console.log(message);
	try{ // store in history
		this.history[this.data[0]].push(message);
		}
		catch (e) {
			console.log(e)
		}
}


// to update interval based (STFE)
Dg800.prototype.generateTelemetryInterval = function () {

    Object.keys(this.state).forEach(function (id) {

        // Real Timestamp
		var timestamp = this.state['Time.stamp'];
		// Artificial timestamp
        //var timestamp= Date.now();

		// built message
		var message = { timestamp: timestamp, value: this.state[id], id: id};
		// notify realtimeserver
        this.notify(message);
		try{ // store in history
			this.history[id].push(message);
			}
			catch (e) {
				console.log(e)
			}
        //this.state["comms.sent"] += JSON.stringify(state).length;

	}, this);
	//console.log(state);
};


// notifiy function, called in generate Telemetry, notifies listeners
Dg800.prototype.notify = function (point) {
    this.listeners.forEach(function (l) {
        l(point);
    });
};


// manages listeners for realtime telemetry
Dg800.prototype.listen = function (listener) {
    this.listeners.push(listener);
    return function () {
        this.listeners = this.listeners.filter(function (l) {
            return l !== listener;
        });
    }.bind(this);
};


// what to do on incoming command
Dg800.prototype.command = function (command) {

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
		function asyncStringify(str) {
			return new Promise((resolve, reject) => {
			  resolve(JSON.stringify(str));
			});
		  }

		asyncStringify(this.history).then(function(write) {//write is the value of the resolved promise in asyncStringify
			fs.writeFile(__dirname + '/saved_logs/DG800_'+seedatetime+'.json', write, (err) => {
				if (err) {
					throw err;
				}
				console.log('History Saved!')
			}) 
		});
	
	};
	
};


module.exports = function () {
    return new Dg800()
};
