

const dgram = require('dgram');
const server = dgram.createSocket('udp4');

server.bind(50011);


function Flutterometer() {
    this.numberIds = 20;
    this.state = {}
    this.timestamp = Date.now();
    
    for (var i = 1; i <= this.numberIds; ++i){
        this.state[`id${i}.Time`] = Date.now();
        this.state[`id${i}.Freq`] = 0;
        this.state[`id${i}.Damp`] = 0;
        
    }
    //console.log(this.state['id3.Time'])

    this.history = {};
    this.listeners = [];
    this.data = new Array();
    Object.keys(this.state).forEach(function (k) {
        this.history[k] = [];
    }, this);

    setInterval(function () {
        this.generateTelemetry();
    }.bind(this), 100);

    this.count = 0
    server.on('message', (msg, rinfo) => {
        this.data = `${msg}`.split(',');

        
		//console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`)
        //console.log(`server got: ${this.data[8]} from ${rinfo.address}:${rinfo.port}`)
        this.count = 0;
        for (var i = 1; i <= this.numberIds; ++i){
            this.state[`id${i}.Time`] = this.data[this.count];
            this.state[`id${i}.Freq`] = this.data[this.count+1];
            this.state[`id${i}.Damp`] = this.data[this.count+2];
            this.count = this.count + 4;

            //console.log(this.state[`id${i}.Time`])
            
        }

    });


    console.log("Flutter-o-Meter initialized!");

};



/**
 * Takes a measurement of spacecraft state, stores in history, and notifies
 * listeners.
 */
Flutterometer.prototype.generateTelemetry = function () {

    Object.keys(this.state).forEach(function (id) {
        
        if (id.slice(4,8) === 'Time'){
			// Actual Timestamp from received data
            this.timestamp = Math.round(this.state[id]*1000);
			// Artificial timestamp
            //this.timestamp= Date.now();
        }
        //console.log(timestamp);
        var state = { timestamp: this.timestamp, value: this.state[id], id: id};
        this.notify(state);
        this.history[id].push(state);
        //this.state["comms.sent"] += JSON.stringify(state).length;
        console.log(state);
        
    }, this);
    
};

Flutterometer.prototype.notify = function (point) {
    this.listeners.forEach(function (l) {
        l(point);
    });
};

Flutterometer.prototype.listen = function (listener) {
    this.listeners.push(listener);
    return function () {
        this.listeners = this.listeners.filter(function (l) {
            return l !== listener;
        });
    }.bind(this);
};

module.exports = function () {
    return new Flutterometer()
};
