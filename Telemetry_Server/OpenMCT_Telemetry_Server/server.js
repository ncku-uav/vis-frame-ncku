/**
 * Basic implementation of a history and realtime server.
 */

// var Spacecraft = require('./spacecraft');
//var TFlex = require('./tflex');
//var Flutterometer = require('./flutterometer');
var Dg800 = require('./DG800');
var Horyzn = require('./horyzn');
var RealtimeServer = require('./realtime-server');
var HistoryServer = require('./history-server');
//var StaticServer = require('./static-server');




// var spacecraft = new Spacecraft();
//var tflex = new TFlex();
//var flutterometer = new Flutterometer();
var dg800 = new Dg800();
//var horyzn = new Horyzn();
// var realtimeServer = new RealtimeServer(spacecraft,8082);
// var historyServer = new HistoryServer(spacecraft, 8081);
//var staticServer = new StaticServer(8080);
//var realtimeServer = new RealtimeServer(tflex,8092);
//var historyServer = new HistoryServer(tflex, 8091);
//var realtimeServer = new RealtimeServer(flutterometer,8102);
//var historyServer = new HistoryServer(flutterometer, 8101);

var realtimeServer = new RealtimeServer(dg800,8112);
var historyServer = new HistoryServer(dg800, 8111);

//var realtimeServer = new RealtimeServer(horyzn,8212);
//var historyServer = new HistoryServer(horyzn, 8211);
