

const dgram = require('dgram');
const server = dgram.createSocket('udp4');

server.bind(50012);


function Dg800() {
    
    this.state = {

        "data.gps.iTOW": 0,
        "data.gps.lon": 0,
        "data.gps.lat" : 0,
        "data.gps.heightMS" : 0,
        "data.gps.gSpeed": 0,
        "data.gps.headingMotion" :0,
        "data.gps.headingVehicle" :0,
		"data.gps.fixType" :0,
        
        "data.nano.vdot" :0,
        "data.nano.v" :0,
		"data.nano.ch1" :0,
		"data.nano.ch2" :0,
		"data.nano.ch3" :0,
		"data.nano.ch4" :0,
		"data.nano.ch5" :0,
		"data.nano.ch6" :0,
		"data.nano.ch7" :0,
		"data.nano.ch8" :0,
		"data.nano.ch9" :0,
		"data.nano.ch10" :0,
		"data.nano.ch11" :0,
		"data.nano.ch12" :0,
		"data.nano.ch13" :0,
        
        "data.strap.roll": 0,
        "data.strap.pitch" :0,
        "data.strap.yaw" :0,
        
        "data.thr.force1" :0,
        "data.thr.force2" :0,
		"data.thr.temp1" :0,
		"data.thr.temp2" :0,
		
		"data.imu.AccX" :0,
		"data.imu.AccY" :0,
		"data.imu.AccZ" :0,
		"data.imu.GyroX" :0,
		"data.imu.GyroY" :0,
		"data.imu.GyroZ" :0,
		
		"data.adp.pstat" :0,
		"data.adp.pdyn" :0,
		"data.adp.AirSpeed" :0,
		
				
		
        
        "Timestamp": Date.now()

    };
    
    
        
    
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
	this.initGPSheight = 0;
    server.on('message', (msg, rinfo) => {
        this.data = `${msg}`.split(',');

        
		//console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`)
        //console.log(`server got: ${this.data[8]} from ${rinfo.address}:${rinfo.port}`)

            this.state[this.data[0]] = this.data[1];
            this.state['Timestamp'] = this.data[2];
			
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
			if (this.data[0] === 'data.gps.heightMS' && this.state['data.gps.fixType'] === '3' && this.count < 100){
			this.initGPSheight = this.data[1]/1000;
			this.count = this.count + 1;
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
        //this.timestamp = Math.round(this.state['Timestamp']*1000);
		// Artificial timestamp
        this.timestamp= Date.now();
    
        //console.log(timestamp);
        var state = { timestamp: this.timestamp, value: this.state[id], id: id};
        this.notify(state);
		//console.log(state);
        this.history[id].push(state);
        //this.state["comms.sent"] += JSON.stringify(state).length;
        
        
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
