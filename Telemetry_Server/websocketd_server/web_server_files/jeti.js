
var request_data_interval
var newline = String.fromCharCode(10);

var TIME_time=[];
var GPS_lat=[];
var GPS_lon=[];
var GPS_alt=[];
var GPS_speed=[];

var JSON_data;

var ALT_plot_ID = document.getElementById('ALT_plot');
var AS_plot_ID = document.getElementById('AS_plot');
var GPS_plot_ID = document.getElementById('GPS_plot');

var DATE_year = 2019;
var DATE_month = 3;
var DATE_day = 1;
var TIME_hour = 0;
var TIME_minute = 0;
var TIME_second = 0;
var TIME_centisecond = 0;

var DATE_set = 0;
var TIME_set = 0;

var current_timestamp = 0;

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
          y: GPS_speed,
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

    if(JSON_data.hasOwnProperty('jeti'))
    {
      if(JSON_data.jeti.hasOwnProperty('DATE'))
      {
        DATE_day = JSON_data.jeti.DATE.day;
        DATE_month = JSON_data.jeti.DATE.month;
        DATE_year = JSON_data.jeti.DATE.year;
        DATE_set = 1;
      }
      if(JSON_data.jeti.hasOwnProperty('TIME'))
      {
        TIME_hour = JSON_data.jeti.TIME.hour;
        TIME_minute = JSON_data.jeti.TIME.minute
        TIME_second = JSON_data.jeti.TIME.second;
        TIME_centisecond = JSON_data.jeti.TIME.hundr;
        TIME_set = 1;
      }
      if(TIME_set == 1 && DATE_set == 1)
      {
        current_timestamp = new Date(DATE_year, DATE_month, DATE_day, TIME_hour, TIME_minute, TIME_second, TIME_centisecond*100).getUnixTime();
      }
	    if(JSON_data.jeti.hasOwnProperty('SPEED') && TIME_set == 1 && DATE_set == 1)
        {
        var update_AS_plot = {
        x:[[current_timestamp]],
        y:[[JSON_data.jeti.SPEED.speed]],
        };
        Plotly.extendTraces(AS_plot_ID, update_AS_plot, [0]);
        }

    	if(JSON_data.jeti.hasOwnProperty('ALT') && TIME_set == 1 && DATE_set == 1)
        {
        var update_ALT_plot = {
        x:[[current_timestamp]],
        y:[[JSON_data.jeti.ALT.alt]],
        };
        Plotly.extendTraces(ALT_plot_ID, update_ALT_plot, [0]);
        }

    	if(JSON_data.jeti.hasOwnProperty('GPS'))
        {
        var update_GPS_plot = {
        lat:[[String(JSON_data.jeti.GPS.lat)]],
        lon:[[String(JSON_data.jeti.GPS.lon)]],
        };
        Plotly.extendTraces(GPS_plot_ID, update_GPS_plot, [0]);
        }

    jeti_get_values();

    }
};

webSocket.onclose = function() {
//  document.writeln('Connection closed.&lt;br&gt;');
};

function jeti_get_values(){
	if(JSON_data.jeti.hasOwnProperty('GPS'))
	{
    document.getElementById('jeti_GPS_age').textContent 		= JSON_data.jeti.GPS.age;
	  document.getElementById('jeti_GPS_lat').textContent 		= JSON_data.jeti.GPS.lat;
	  document.getElementById('jeti_GPS_lon').textContent 		= JSON_data.jeti.GPS.lon;
    if(JSON_data.jeti.GPS.hasOwnProperty('DIST'))
    {
      document.getElementById('jeti_GPS_DIST_dist').textContent 		= JSON_data.jeti.GPS.DIST.dist;
      document.getElementById('jeti_GPS_DIST_course').textContent 	= JSON_data.jeti.GPS.DIST.course;
      document.getElementById('jeti_GPS_DIST_card').textContent 		= JSON_data.jeti.GPS.DIST.card;
    }
	}
}
