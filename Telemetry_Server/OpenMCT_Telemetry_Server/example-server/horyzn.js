

const dgram = require('dgram');
const server = dgram.createSocket('udp4');

server.bind(50012);

function Horyzn() {
    this.state = {

        "GPS.alt" : 0,
        "GPS.satcount" : 0,
        "Speed.airspeed": 0,
        "GPS.speed" :0,
        "ACC.x" :0,
        "ACC.y" :0,
        "ACC.z" :0,
	"Mag.x" :0,
	"Mag.y" :0,
	"Mag.z" :0,
	"Batt.V":0,
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



    console.log("Horyzn launched!");

    server.on('message', (msg, rinfo) => {
        this.data = `${msg}`.split(',');

        console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`)
        //console.log(`server got: ${this.data[8]} from ${rinfo.address}:${rinfo.port}`)

        this.state["GPS.alt"] = this.data[0];
        this.state["GPS.satcount"] = this.data[1];
        this.state["Speed.airspeed"] = this.data[2];
        this.state["GPS.speed"] = this.data[3];
        this.state["ACC.x"] = this.data[4];
        this.state["ACC.y"] = this.data[5];
        this.state["ACC.z"] = this.data[6];
        this.state["Mag.x"] = this.data[7];
	this.state["Mag.y"] = this.data[8];
	this.state["Mag.z"] = this.data[9];
	this.state["Batt.V"] = this.data[10];

    });

};



/**
 * Takes a measurement of spacecraft state, stores in history, and notifies
 * listeners.
 */
Horyzn.prototype.generateTelemetry = function () {
    var timestamp = this.state["Time.stamp"]*1000, sent = 0; //= Date.now(), sent = 0;
    Object.keys(this.state).forEach(function (id) {
        var state = { timestamp: timestamp, value: this.state[id], id: id};
        this.notify(state);
        this.history[id].push(state);
        //this.state["comms.sent"] += JSON.stringify(state).length;
    }, this);
};

Horyzn.prototype.notify = function (point) {
    this.listeners.forEach(function (l) {
        l(point);
    });
};

Horyzn.prototype.listen = function (listener) {
    this.listeners.push(listener);
    return function () {
        this.listeners = this.listeners.filter(function (l) {
            return l !== listener;
        });
    }.bind(this);
};

module.exports = function () {
    return new Horyzn()
};
