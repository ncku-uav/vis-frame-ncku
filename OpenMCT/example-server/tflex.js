/*
 Spacecraft.js simulates a small spacecraft generating telemetry.
*/

const dgram = require('dgram');
const server = dgram.createSocket('udp4');

server.bind(50011)

function TFlex() {
    this.state = {

        "comms.sent": 0,
        "Check.no": 0,
        "Att.pitch" : 0,
        "Att.roll" : 0,
        "GPS.alt": 0,
        "GPS.speed" :0,
        "Time.stamp": Date.now()
    };
    this.history = {};
    this.listeners = [];
    this.data = new Array();
    Object.keys(this.state).forEach(function (k) {
        this.history[k] = [];
    }, this);

    setInterval(function () {
        this.generateTelemetry();
    }.bind(this), 10);

    

    console.log("Example T-FLEX launched!");

    server.on('message', (msg, rinfo) => {
        this.data = `${msg}`.split(',');
    
        //console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`)
    
        this.state["Att.pitch"] = this.data[0];
        this.state["Att.roll"] = this.data[1];
        this.state["GPS.speed"] = this.data[2];
        this.state["GPS.alt"] = this.data[3];
        this.state["Check.no"] = this.data[4];
        this.state["Time.stamp"] = this.data[5]*1000;

    });

};

    

/**
 * Takes a measurement of spacecraft state, stores in history, and notifies 
 * listeners.
 */
TFlex.prototype.generateTelemetry = function () {
    var timestamp = this.state["Time.stamp"], sent = 0;
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
