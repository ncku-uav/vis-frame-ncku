
var request_data_interval
var newline = String.fromCharCode(10);

var TIME_time=[];
var GPS_lat=[];
var GPS_lon=[];
var GPS_alt=[];
var ASS_airspeed=[];

var JSON_data;

var ALT_plot_ID = document.getElementById('ALT_plot');
var AS_plot_ID = document.getElementById('AS_plot');
var GPS_plot_ID = document.getElementById('GPS_plot');

var mapbox_access_token = 'pk.eyJ1IjoidHRvIiwiYSI6ImNqc3dkMzF2dTA5MnQ0M29maDQ3YTQyMmUifQ.ovQV_NNwcJ2DJwy8SwmveA';

var url = window.location.host
webSocket = new WebSocket('ws://'+url+':443');

webSocket.onopen = function() {

      var GPS_plot_data = {
        lat: GPS_lat,
        lon: GPS_lon,
        type:'scattermapbox',
//        mode: 'lines',
      };

      var GPS_plot_layout = {
	autosize: true,
	hovermode: 'closest',
        dragmode: 'zoom',
        mapbox: {
          center: {
            lat: 48.33,
            lon: 11.76,
          },
          domain: {
            x: [0, 1],
            y: [0, 1]
          },
          style: 'dark',
          zoom: 10 //14
        },
        margin: {
          r: 0,
          t: 0,
          b: 0,
          l: 0,
          pad: 1
        },
        paper_bgcolor: '#191A1A',
        plot_bgcolor: '#191A1A',
        showlegend: false
      };


      Plotly.setPlotConfig({
        mapboxAccessToken: mapbox_access_token,
      });

      Plotly.plot(GPS_plot_ID, [GPS_plot_data], GPS_plot_layout, {showLink: false});


      var ALT_plot_data = {
          x: TIME_time,
          y: GPS_alt,
          type: 'scatter',
          fill: 'tozeroy',
      };

      var ALT_plot_layout = {
          xaxis: {
            title: 'Time',
            showgrid: false
          },
          yaxis: {
            title: 'Altitude [m]'
          },
          margin: {
            r: 0,
            t: 0,
            b: 0,
            l: 0,
            pad: 1
          },
          paper_bgcolor: '#191A1A',
          plot_bgcolor: '#191A1A',
          showlegend: true

      };

      Plotly.plot(ALT_plot_ID, [ALT_plot_data], ALT_plot_layout, {showLink: false});


      var AS_plot_data = {
          x: TIME_time,
          y: ASS_airspeed,
          type: 'scatter'
      };

      var AS_plot_layout = {
          xaxis: {
            title: 'Time',
            showgrid: false
          },
          yaxis: {
            title: 'Airspeed [m/s]'
          },
          margin: {
            r: 0,
            t: 0,
            b: 0,
            l: 0,
            pad: 0
          },
          paper_bgcolor: '#191A1A',
          plot_bgcolor: '#191A1A',
          showlegend: true

      };

      Plotly.plot(AS_plot_ID, [AS_plot_data], AS_plot_layout, {showLink: false});


};


webSocket.onmessage = function(event){
    stats = event.data;
    console.log(stats);
    JSON_data = JSON.parse(stats);

    if(JSON_data.hasOwnProperty('frsky'))
    {
	if(JSON_data.frsky.hasOwnProperty('ASS'))
    {
    var update_AS_plot = {
    x:[[JSON_data.frsky.TIME]],
    y:[[JSON_data.frsky.ASS.airspeed]],
    };
    Plotly.extendTraces(AS_plot_ID, update_AS_plot, [0]);
    }

	if(JSON_data.frsky.hasOwnProperty('GPS'))
    {
    var update_ALT_plot = {
    x:[[JSON_data.frsky.TIME]],
    y:[[JSON_data.frsky.GPS.alt]],
    };
    Plotly.extendTraces(ALT_plot_ID, update_ALT_plot, [0]);
    }

	if(JSON_data.frsky.hasOwnProperty('GPS'))
    {
    var update_GPS_plot = {
    lat:[[String(JSON_data.frsky.GPS.lat)]],
    lon:[[String(JSON_data.frsky.GPS.lon)]],
    };
    Plotly.extendTraces(GPS_plot_ID, update_GPS_plot, [0]);
    }

    frsky_get_values();

    }
};

webSocket.onclose = function() {
//  document.writeln('Connection closed.&lt;br&gt;');
};

function frsky_get_values(){
	if(JSON_data.frsky.hasOwnProperty('TIME'))
	{
	document.getElementById('frsky_TIME_time').textContent 		= JSON_data.frsky.TIME;
	}
	if(JSON_data.frsky.hasOwnProperty('RSSI'))
	{
	document.getElementById('frsky_RSSI_rssi').textContent 		= JSON_data.frsky.RSSI.rssi;
	document.getElementById('frsky_RSSI_adc1').textContent 		= JSON_data.frsky.RSSI.adc1;
	document.getElementById('frsky_RSSI_adc2').textContent 		= JSON_data.frsky.RSSI.adc2;
	document.getElementById('frsky_RSSI_rxBatt').textContent 	= JSON_data.frsky.RSSI.rxBatt;
	document.getElementById('frsky_RSSI_swr').textContent 		= JSON_data.frsky.RSSI.swr;
	}

	if(JSON_data.frsky.hasOwnProperty('ASS'))
	{
	document.getElementById('frsky_ASS_airspeed').textContent 	= JSON_data.frsky.ASS.airspeed;
	}

	if(JSON_data.frsky.hasOwnProperty('FCS'))
	{
	document.getElementById('frsky_FCS_current').textContent 	= JSON_data.frsky.FCS.current;
	document.getElementById('frsky_FCS_voltage').textContent 	= JSON_data.frsky.FCS.voltage;
	}

	if(JSON_data.frsky.hasOwnProperty('FLVSS'))
	{
	document.getElementById('frsky_FLVSS_V0').textContent 		= JSON_data.frsky.FLVSS.V0;
	document.getElementById('frsky_FLVSS_V1').textContent 		= JSON_data.frsky.FLVSS.V1;
	document.getElementById('frsky_FLVSS_V2').textContent 		= JSON_data.frsky.FLVSS.V2;
	document.getElementById('frsky_FLVSS_V3').textContent 		= JSON_data.frsky.FLVSS.V3;
	document.getElementById('frsky_FLVSS_V4').textContent 		= JSON_data.frsky.FLVSS.V4;
	document.getElementById('frsky_FLVSS_V5').textContent 		= JSON_data.frsky.FLVSS.V5;
	}

	if(JSON_data.frsky.hasOwnProperty('GPS'))
	{
	document.getElementById('frsky_GPS_lat').textContent 		= JSON_data.frsky.GPS.lat;
	document.getElementById('frsky_GPS_lon').textContent 		= JSON_data.frsky.GPS.lon;
	document.getElementById('frsky_GPS_alt').textContent 		= JSON_data.frsky.GPS.alt;
	document.getElementById('frsky_GPS_speed').textContent 		= JSON_data.frsky.GPS.speed;
	document.getElementById('frsky_GPS_cog').textContent 		= JSON_data.frsky.GPS.cog;
	document.getElementById('frsky_GPS_date').textContent 		= JSON_data.frsky.GPS.day+"."+JSON_data.frsky.GPS.month+"."+JSON_data.frsky.GPS.year;
	document.getElementById('frsky_GPS_time').textContent 		= JSON_data.frsky.GPS.hour+":"+JSON_data.frsky.GPS.min+":"+JSON_data.frsky.GPS.sec;
	}

	if(JSON_data.frsky.hasOwnProperty('RPM'))
	{
	document.getElementById('frsky_RPM_rpm').textContent 		= JSON_data.frsky.RPM.rpm;
	document.getElementById('frsky_RPM_t1').textContent 		= JSON_data.frsky.RPM.t1;
	document.getElementById('frsky_RPM_t2').textContent 		= JSON_data.frsky.RPM.t2;
	}

	if(JSON_data.frsky.hasOwnProperty('SP2UART'))
	{
	document.getElementById('frsky_SP2UART_adc3').textContent 	= JSON_data.frsky.SP2UART.adc3;
	document.getElementById('frsky_SP2UART_adc4').textContent 	= JSON_data.frsky.SP2UART.adc4;
	}

	if(JSON_data.frsky.hasOwnProperty('VARIO'))
	{
	document.getElementById('frsky_VARIO_alt').textContent 		= JSON_data.frsky.VARIO.alt;
	document.getElementById('frsky_VARIO_vsi').textContent 		= JSON_data.frsky.VARIO.vsi;
	}

	if(JSON_data.frsky.hasOwnProperty('SPEC1'))
	{
	document.getElementById('frsky_SPEC1_V0').textContent 		= JSON_data.frsky.SPEC1.V0;
	document.getElementById('frsky_SPEC1_V1').textContent 		= JSON_data.frsky.SPEC1.V1;
	document.getElementById('frsky_SPEC1_V2').textContent 		= JSON_data.frsky.SPEC1.V2;
	document.getElementById('frsky_SPEC1_V3').textContent 		= JSON_data.frsky.SPEC1.V3;
	document.getElementById('frsky_SPEC1_V4').textContent 		= JSON_data.frsky.SPEC1.V4;
	document.getElementById('frsky_SPEC1_V5').textContent 		= JSON_data.frsky.SPEC1.V5;
	}
}