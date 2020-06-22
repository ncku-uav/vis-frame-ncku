/**
 * Basic implementation of a history and realtime server.
 */

var Spacecraft = require('./spacecraft');
var TFlex = require('./tflex');
var RealtimeServer = require('./realtime-server');
var HistoryServer = require('./history-server');
var StaticServer = require('./static-server');




var spacecraft = new Spacecraft();
var tflex = new TFlex();
var realtimeServer = new RealtimeServer(spacecraft,8082);
var historyServer = new HistoryServer(spacecraft, 8081);
var staticServer = new StaticServer(8080);
var realtimeServer = new RealtimeServer(tflex,8092);
var historyServer = new HistoryServer(tflex, 8091);
