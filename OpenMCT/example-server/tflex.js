/*
 Spacecraft.js simulates a small spacecraft generating telemetry.
*/

const dgram = require('dgram');
const server = dgram.createSocket('udp4');

server.bind(50011)

function TFlex() {
    this.state = {

        "prop.fuel": 77,
        "prop.thrusters": "OFF",
        "comms.recd": 0,
        "comms.sent": 0,
        "pwr.temp": 245,
        "pwr.c": 8.15,
        "pwr.v": 30,

        "Att.pitch" : 0,
        "Att.roll" : 0,
        "GPS.alt": 0,
        "GPS.speed" :0
    };
    this.history = {};
    this.listeners = [];
    this.data = new Array();
    Object.keys(this.state).forEach(function (k) {
        this.history[k] = [];
    }, this);

    setInterval(function () {
        this.updateState();
        this.generateTelemetry();
    }.bind(this), 1);

    

    console.log("Example T-FLEX launched!");

};

TFlex.prototype.updateState = function () {

//    this.state["Att.pitch"] = 20 + Math.random()*2;
//    this.state["Att.roll"] = 20 + Math.random()*2;
//    this.state["GPS.alt"] = 100 + Math.random()*2;
//    this.state["GPS.speed"] = 20 + Math.random()*2;
    
    server.on('message', (msg, rinfo) => {
                                        this.data = `${msg}`.split(',');

                                        this.state["Att.pitch"] = this.data[0];
                                        this.state["Att.roll"] = this.data[1];
                                        this.state["GPS.alt"] = this.data[3];
                                        this.state["GPS.speed"] = this.data[4];
                                        //console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`)
        
});
    //server.on('message', (msg, rinfo) => {console.log(`server got: ${parseInt(msg} from ${rinfo.address}:${rinfo.port}`)});
  	//console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
	//});
};

/**
 * Takes a measurement of spacecraft state, stores in history, and notifies 
 * listeners.
 */
TFlex.prototype.generateTelemetry = function () {
    var timestamp = Date.now(), sent = 0;
    Object.keys(this.state).forEach(function (id) {
        var state = { timestamp: timestamp, value: this.state[id], id: id};
        this.notify(state);
        this.history[id].push(state);
        this.state["comms.sent"] += JSON.stringify(state).length;
    }, this);
};

TFlex.prototype.notify = function (point) {
    this.listeners.forEach(function (l) {
        l(point);
    });
};

TFlex.prototype.listen = function (listener) {
    this.listeners.push(listener);
    return function () {
        this.listeners = this.listeners.filter(function (l) {
            return l !== listener;
        });
    }.bind(this);
};

module.exports = function () {
    return new TFlex()
};