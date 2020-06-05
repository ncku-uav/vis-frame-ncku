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
        "GPS.alt": 0
    };
    this.history = {};
    this.listeners = [];
    Object.keys(this.state).forEach(function (k) {
        this.history[k] = [];
    }, this);

    setInterval(function () {
        this.updateState();
        this.generateTelemetry();
    }.bind(this), 1000);

    console.log("Example T-FLEX launched!");

};

TFlex.prototype.updateState = function () {

    //this.state["GPS.alt"] = 100 + Math.random()*2;
    server.on('message', (msg, rinfo) => {this.state["GPS.alt"] = parseInt(msg)});
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
