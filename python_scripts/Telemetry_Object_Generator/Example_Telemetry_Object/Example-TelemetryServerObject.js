

const dgram = require('dgram');
const server = dgram.createSocket('udp4');

server.bind(50012);


function Example() {

	// initialize Data Parameters
    this.state = {"key.0": 0};


	//Initialize working Parameters and Object
    this.history = {};
    this.listeners = [];
	this.data = new Array();
	Object.keys(this.state).forEach(function (k) {
        this.history[k] = [];
	}, this);

	//Initialize Interval for Notifiing OpenMCT about new Telemetry
    setInterval(function () {
        this.generateTelemetry();
    }.bind(this), 50); //z.B. 100ms

 
	//what to do, when a message from the UDP Port arrives
    server.on('message', (msg, rinfo) => {
		//parse the data
		this.data = `${msg}`.split(',');
		

		
		//console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`)
		
		//Save the data to the state array
		this.state['key.0'] = this.data[1];
	
		//CALCULATIONS HERE
		// EXAMPLE
		if (this.data[0] === 'key.0'){
			//calculate stuff
		}
			
    });


    console.log("Example initialized!");

};


/**
 * Takes a measurement of state, stores in history, and notifies
 * listeners.
 */
Example.prototype.generateTelemetry = function () {

    Object.keys(this.state).forEach(function (id) {

        // If you have a real timestamp: 
		// this.timestamp = this.state['Time.stamp'];

		// Artificial timestamp
        this.timestamp= Date.now();

        //console.log(timestamp);
        var state = { timestamp: this.timestamp, value: this.state[id], id: id};
        this.notify(state);
		//console.log(state);
        try{
			this.history[id].push(state);
			}
			catch (e) {
				console.log(e)
			}
        //this.state["comms.sent"] += JSON.stringify(state).length; //package counter


    }, this);

};

// notifiy function, called in generate Telemetry, notifies listeners
Example.prototype.notify = function (point) {
    this.listeners.forEach(function (l) {
        l(point);
    });
};

// 
Example.prototype.listen = function (listener) {
    this.listeners.push(listener);
    return function () {
        this.listeners = this.listeners.filter(function (l) {
            return l !== listener;
        });
    }.bind(this);
};

module.exports = function () {
    return new Example()
};
