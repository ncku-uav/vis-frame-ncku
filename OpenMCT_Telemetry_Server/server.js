/**
 * Basic implementation of a history and realtime server.
 */

// var Spacecraft = require('./spacecraft');
//var TFlex = require('./tflex');
var Flutterometer = require('./flutterometer');
var Dg800 = require('./DG800');
// var Horyzn = require('./horyzn');
var RealtimeServer = require('./realtime-server');
var HistoryServer = require('./history-server');
//var StaticServer = require('./static-server');

var expressWs = require('express-ws');
var app = require('express')();
expressWs(app);



// var spacecraft = new Spacecraft();
//var tflex = new TFlex();
var flutterometer = new Flutterometer();
var dg800 = new Dg800();
//var horyzn = new Horyzn();
// var realtimeServer = new RealtimeServer(spacecraft,8082);
// var historyServer = new HistoryServer(spacecraft, 8081);
//var staticServer = new StaticServer(8080);
//var realtimeServer = new RealtimeServer(tflex,8092);
//var historyServer = new HistoryServer(tflex, 8091);
//var realtimeServer = new RealtimeServer(flutterometer,8102);
//var historyServer = new HistoryServer(flutterometer, 8101);

var realtimeServerFLIPASED = new RealtimeServer(flutterometer);
var historyServerFLIPASED = new HistoryServer(flutterometer);

var realtimeServerDG800 = new RealtimeServer(dg800);
var historyServerDG800 = new HistoryServer(dg800);

//var realtimeServer = new RealtimeServer(horyzn,8212);
//var historyServer = new HistoryServer(horyzn, 8211);

app.use('/FLUTTERRealtime', realtimeServerFLIPASED);
app.use('/FLUTTERHistory', historyServerFLIPASED);

app.use('/DG800Realtime', realtimeServerDG800);
app.use('/DG800History', historyServerDG800);
var port = process.env.PORT || 16969

app.listen(port)
