

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
        //this.generateTelemetryInterval();
    }.bind(this), 100);



	server.on('message', (msg, rinfo) => {
        //parse the data
		this.data = `${msg}`.split(',');
		
		// Check server message
		//console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`)
        //console.log(`server got: ${this.data[8]} from ${rinfo.address}:${rinfo.port}`)
		
		// Error Flags are set before setting the state in order to make a logic to only print them once when set
		if (this.data[0].search('Flag') !== -1){
			this.ErrorFlags("Flag.RxMux1.Autopilot", "RXMUX 1 Autopilot On!");
			this.ErrorFlags("Flag.RxMux1.SPI", "RXMUX 1 SPI communication fail!");      
			this.ErrorFlags("Flag.RxMux1.ECU_PIN", "RXMUX 1 ECU Pin Connected!");   
			this.ErrorFlags("Flag.RxMux1.ECU_PPM", "RXMUX 1 ECU is commanded to turn on!");    
			this.ErrorFlags("Flag.RxMux1.JETI", "RXMUX 1 Reception from Jeti!");    
			this.ErrorFlags("Flag.RxMux1.GRP", "RXMUX 1 Reception from Graupner!");      
			//this.ErrorFlags("Flag.RxMux1.c_JETI", "ECU RxMux1 c_JETI!");   
			//this.ErrorFlags("Flag.RxMux1.c_GRP", "RxMux 1 c_GRP!");

			this.ErrorFlags("Flag.RxMux2.Autopilot", "RXMUX 2 Autopilot On!");
			this.ErrorFlags("Flag.RxMux2.SPI", "RXMUX 2 SPI communication fail!");      
			this.ErrorFlags("Flag.RxMux2.ECU_PIN", "RXMUX 2 ECU Pin Connected!");   
			this.ErrorFlags("Flag.RxMux2.ECU_PPM", "RXMUX 2 ECU is commanded to turn on!");    
			this.ErrorFlags("Flag.RxMux2.JETI", "RXMUX 2 Reception from Jeti!");    
			this.ErrorFlags("Flag.RxMux2.GRP", "RXMUX 2 Reception from Graupner!");      
			//this.ErrorFlags("Flag.RxMux2.c_JETI", "ECU RxMux2 c_JETI!");   
			//this.ErrorFlags("Flag.RxMux2.c_GRP", "RxMux 2 c_GRP!");
			
			this.ErrorFlags("Flag.RxMux3.Autopilot", "RXMUX 3 Autopilot On!");
			this.ErrorFlags("Flag.RxMux3.SPI", "RXMUX 3 SPI communication fail!");      
			this.ErrorFlags("Flag.RxMux3.ECU_PIN", "RXMUX 3 ECU Pin Connected!");   
			this.ErrorFlags("Flag.RxMux3.ECU_PPM", "RXMUX 3 ECU is commanded to turn on!");    
			this.ErrorFlags("Flag.RxMux3.JETI", "RXMUX 3 Reception from Jeti!");    
			this.ErrorFlags("Flag.RxMux3.GRP", "RXMUX 3 Reception from Graupner!");      
			//this.ErrorFlags("Flag.RxMux1.c_JETI", "ECU RxMux1 c_JETI!");   
			//this.ErrorFlags("Flag.RxMux1.c_GRP", "RxMux 1 c_GRP!");

			this.ErrorFlags("Flag.flightHAT.XsensConfigFault", "Xsens Config Fault!");   //49   
			this.ErrorFlags("Flag.flightHAT.XsensUnexpectedConfigAck", "Xsens Unexpected Config Ack!");  //51
			this.ErrorFlags("Flag.flightHAT.XsensConfigChecksumFault", "Xsens Config Checksum Fault!");  //53
			this.ErrorFlags("Flag.flightHAT.XsensConfigReceiveFault", "Xsens Config Receive Fault!");   //55
			this.ErrorFlags("Flag.flightHAT.XsensWrongHeader", "Xsens Wrong Header!");      //57
			this.ErrorFlags("Flag.flightHAT.XsensChecksumFault", "Xsens Checksum Fault!");    //59
			this.ErrorFlags("Flag.flightHAT.XsensDataTypeError", "Xsens Data Type Error!");    //61
			this.ErrorFlags("Flag.flightHAT.XsensUnexpectedByteIndex", "Xsens Unexpected Byte Index!");  //63
			this.ErrorFlags("Flag.flightHAT.XsensUnexpectedDataID", "Xsens Unexpected Data ID!"); //65
			this.ErrorFlags("Flag.flightHAT.ADSConfigFault", "ADS Config Fault!");        //67
			this.ErrorFlags("Flag.flightHAT.ADSChecksumFault", "ADS Checksum Fault!");      //69
			this.ErrorFlags("Flag.flightHAT.ADSTooMuchDataTypes", "ADS Too Much Data Types!");   //71
			this.ErrorFlags("Flag.flightHAT.IMUWrongRequestID", "IMU Wrong Request ID!");     //73
			this.ErrorFlags("Flag.flightHAT.IMUWrongID", "IMU Wrong ID!");            //75
			this.ErrorFlags("Flag.flightHAT.SHMWrongID", "SHM Wrong ID!");            //77
			this.ErrorFlags("Flag.flightHAT.WrongRXMUXChecksum", "Wrong RXMUX Checksum!");   //79
			
			return;
		}

		//Save the data to the state array
		this.state[this.data[0]] = this.data[1];
		this.state['Time.stamp'] = Math.round(this.data[2]*1000); //convert python timestamp[s] to JS timestamp [ms]
		
		//console.log(Date.now()-this.state['Time.stamp']) //check lag incomming
		//console.log(this.state['Time.stamp']) //check Timestamp
		//console.log(this.state[this.data[0]]) //check paylaod
		
		//CALCULATIONS

        
            if (this.data[0] === "Bat.FlightHead"){
                this.state[this.data[0]] = this.data[1]*0.0062;
                //console.log(this.state[this.data[0]])
			}

			if (this.data[0] === "Bat.RX_MUX1" || this.data[0] === "Bat.RX_MUX2" || this.data[0] === "Bat.RX_MUX3"){
                this.state[this.data[0]] = 0.0021 * this.data[1] - 0.1366;
                //console.log(this.state[this.data[0]])
			}

			if (this.data[0] === "xSens.AccX" || this.data[0] === "xSens.AccY" || this.data[0] === "xSens.AccZ"){
                this.state[this.data[0]] = this.data[1]/9.806;
                //console.log(this.state[this.data[0]])
			}

			if (this.data[0] === "xSens.AccZ"){
                this.state[this.data[0]] = -this.data[1]/9.806;
                //console.log(this.state[this.data[0]])
			}
			
			
			
			     
			//console.log(this.data[0])
			//console.log(this.data[1])

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

TFlex.prototype.ErrorFlags = function (key, message) {

	if (this.data[0] === key && this.data[1] === '1' && this.state[this.data[0]] !== message){
		this.state[this.data[0]] = message;
		this.generateTelemetry();
		return true;
	} else if (this.data[0] === key && this.data[1] === '1' && this.state[this.data[0]] === message){
		return true;
	} else if (this.data[0] === key && this.data[1] === '0' && this.state[this.data[0]] !== message){
		return true;
	}else if (this.data[0] === key && this.data[1] === '0' && this.state[this.data[0]] === message){
		this.state[this.data[0]] = this.state[this.data[0]]+' Opposite!';
		this.generateTelemetry();
		return true;
	}else{
		return false;
	}

}



// to update every time new data comes in
TFlex.prototype.generateTelemetry = function () {

	// Real Timestamp
	var timestamp = this.state['Time.stamp'];
	// Artificial timestamp
	//var timestamp= Date.now();

	// built message
	var message = { timestamp: timestamp, value: this.state[this.data[0]], id: this.data[0]};
	// notify realtimeserver
	this.notify(message);
	//console.log(message);
	try{ // store in history
		this.history[this.data[0]].push(message);
		}
		catch (e) {
			//console.log(message)
			//console.log(e)
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
