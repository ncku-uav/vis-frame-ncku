

const dgram = require('dgram');
const server = dgram.createSocket('udp4');

server.bind(50012);


function Dg800() {
    
    this.state = {

        "data.gps.iTOW": 0,
        "data.gps.lon": 0,
        "data.gps.lat" : 0,
        "data.gps.heightMS" : 0,
        "data.gps.gspeed": 0,
        "data.gps.headingMotion" :0,
        "data.gps.headingVehicle" :0,
        
        "data.nano.vdot" :0,
        "data.nano.v" :0,
        
        "data.strap.roll": 0,
        "data.strap.pitch" :0,
        "data.strap.yaw" :0,
        
        "data.thr.force1" :0,
        "data.thr.force2" :0,
        
        "Timestamp": Date.now()

    };
    this.timestamp = Date.now();
    
        
    
    //console.log(this.state['id3.Time'])

    this.history = {};
    this.listeners = [];
    this.data = new Array();
    Object.keys(this.state).forEach(function (k) {
        this.history[k] = [];
    }, this);

    setInterval(function () {
        this.generateTelemetry();
    }.bind(this), 2000);

    this.count = 0
    server.on('message', (msg, rinfo) => {
        this.data = `${msg}`.split(',');

        
		//console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`)
        //console.log(`server got: ${this.data[8]} from ${rinfo.address}:${rinfo.port}`)

            this.state[this.data[0]] = this.data[1];
            this.state['Timestamp'] = this.data[2];

            console.log(this.state);
            
        

    });


    console.log("DG-800 initialized!");

};



/**
 * Takes a measurement of spacecraft state, stores in history, and notifies
 * listeners.
 */
Dg800.prototype.generateTelemetry = function () {

    Object.keys(this.state).forEach(function (id) {
        
        // Real Timestamp
        this.timestamp = Math.round(this.state['Timestamp']*1000);
		// Artificial timestamp
        //this.timestamp= Date.now();
    
        //console.log(timestamp);
        var state = { timestamp: this.timestamp, value: this.state[id], id: id};
        this.notify(state);
        this.history[id].push(state);
        //this.state["comms.sent"] += JSON.stringify(state).length;
        console.log(state);
        
    }, this);
    
};

Dg800.prototype.notify = function (point) {
    this.listeners.forEach(function (l) {
        l(point);
    });
};

Dg800.prototype.listen = function (listener) {
    this.listeners.push(listener);
    return function () {
        this.listeners = this.listeners.filter(function (l) {
            return l !== listener;
        });
    }.bind(this);
};

module.exports = function () {
    return new Dg800()
};
