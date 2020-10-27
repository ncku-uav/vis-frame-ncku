

const dgram = require('dgram');
const server = dgram.createSocket('udp4');
const fs = require('fs');


server.bind(50011);

function TFlex() {
// Initialize working parameters and objects

//read the keys from dictionary
        let rawDict = fs.readFileSync('../openmct/example/FLEXOP/TFLEXdictionary.json')
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

    setInterval(function () {
        //this.generateTelemetry();
    }.bind(this), 50);



    console.log("T-FLEX initiated!");

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

        
            if (this.data[0] === "Bat.FlightHead"){
                this.state[this.data[0]] = this.data[1]*0.0062;
                console.log(this.state[this.data[0]])
            }
        
            console.log(this.state)

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
    console.log("TFLEX initialized!");

};



// to update every time new data comes in
TFlex.prototype.generateTelemetry = function () {

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
TFlex.prototype.generateTelemetryInterval = function () {

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
TFlex.prototype.notify = function (point) {
    this.listeners.forEach(function (l) {
        l(point);
    });
};


// manages listeners for realtime telemetry
TFlex.prototype.listen = function (listener) {
    this.listeners.push(listener);
    return function () {
        this.listeners = this.listeners.filter(function (l) {
            return l !== listener;
        });
    }.bind(this);
};


// what to do on incoming command
TFlex.prototype.command = function (command) {
	if(command === ':startLog'){

		var date = new Date();
		var year = date.getFullYear();
		var month = date.getMonth() + 1;      // "+ 1" becouse the 1st month is 0
		var day = date.getDate();
		var hour = date.getHours();
		var minutes = date.getMinutes();
		var secconds = date.getSeconds();
		var seedatetime = year+ '-'+ month+ '-'+ day+ ' '+ hour+ ':'+ minutes+ ':'+ secconds;
		
		const write = JSON.stringify(this.history)
		fs.writeFile('saved_logs/DG800_'+seedatetime+'.json', write, (err) => {
			if (err) {
				throw err;
			}
		console.log("History saved!")
		});
	}
	
};

module.exports = function () {
    return new TFlex()
};
