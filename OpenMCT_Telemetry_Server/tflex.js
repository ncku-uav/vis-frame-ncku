

const dgram = require('dgram');
const server = dgram.createSocket('udp4');

server.bind(50011);

function TFlex() {

    this.state = {

       
        "Bat.RX_MUX1" :0,
        "Bat.RX_MUX2" :0,
        "Bat.RX_MUX3" :0,
        "Bat.FlightHead" :0,
        "err.Flag" :0,
        'xSens.Hour':0,
        'xSens.Minute':      0, 
        'xSens.Second':      0, 
        'xSens.Millisecond':0, 
        'xSens.EulerA':          0,
        'xSens.EulerB':          0,
        'xSens.EulerY':          0, 
        'xSens.ECEFX':          0,
        'xSens.ECEFY':          0,
        'xSens.ECEFZ':          0, 
        'xSens.AngularVa':      0,
        'xSens.AngularVb':      0,
        'xSens.AngularVy':      0, 
        'xSens.AccX':         0,
        'xSens.AccY':         0,
        'xSens.AccZ':         0, 
        'xSens.VelX':             0,
        'xSens.VelX':             0, 
        'xSens.Sec':            0, 
        'xSens.Latency':        0,
        "Time.stamp": Date.now()

    };
    this.history = {};
    this.listeners = [];
    this.data = new Array();
    Object.keys(this.state).forEach(function (k) {
        this.history[k] = [];
    }, this);

    setInterval(function () {
        //this.generateTelemetry();
    }.bind(this), 50);



    console.log("T-FLEX initiated!");

    server.on('message', (msg, rinfo) => {
        this.data = `${msg}`.split(',');

        //console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`)
        //console.log(`server got: ${this.data[8]} from ${rinfo.address}:${rinfo.port}`)
        this.state[this.data[0]] = this.data[1];
		this.state['Time.stamp'] = Math.round(this.data[2]*1000); //convert python timestamp[s] to JS timestamp [ms]
		//var start = Date.now();
		//console.log(Date.now()-this.state['Time.stamp']) //check lag
		//console.log(this.state['Time.stamp']) //check Timestamp
		//console.log(this.data[1]) //check paylaod

        // this.state["Att.pitch"] = this.data[0];
        // this.state["Att.roll"] = this.data[1];
        // this.state["GPS.speed"] = this.data[2];
        // this.state["GPS.alt"] = this.data[3];
        // this.state["ACC.x"] = this.data[4];
        // this.state["ACC.y"] = this.data[5];
        // this.state["ACC.z"] = this.data[6];
        // this.state["Check.no"] = this.data[7];
        // this.state["Time.stamp"] = this.data[8];
        
        if (this.data[0] === "Bat.FlightHead"){
            this.state[this.data[0]] = this.data[1]*0.0062;
            console.log(this.state[this.data[0]])
        }
        console.log(this.state)
        this.generateTelemetry()
    });

};



/**
 * Takes a measurement of spacecraft state, stores in history, and notifies
 * listeners.
 */
TFlex.prototype.generateTelemetry = function () {
    var timestamp = Date.now()//this.state["Time.stamp"]*1000, sent = 0; //= Date.now(), sent = 0;

    Object.keys(this.state).forEach(function (id) {
        var state = { timestamp: timestamp, value: this.state[id], id: id};
        this.notify(state);
        try{
            //console.log(state)
            this.history[id].push(state);
            }
            catch (e) {
                console.log(e)
            }
        //this.state["comms.sent"] += JSON.stringify(state).length;
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
