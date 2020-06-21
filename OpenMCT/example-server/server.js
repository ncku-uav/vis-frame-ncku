/**
 * Basic implementation of a history and realtime server.
 */

// var Spacecraft = require('./spacecraft');
// var TFlex = require('./tflex');
// var RealtimeServer = require('./realtime-server');
// var HistoryServer = require('./history-server');
var StaticServer = require('./static-server');
// var HistoryServerTflex = require('./history-server-tflex');
// var RealtimeServerTflex = require('./realtime-server-tflex');

var expressWs = require('express-ws');
var app = require('express')();
expressWs(app);


// var spacecraft = new Spacecraft();
// var tflex = new TFlex();
// var realtimeServer = new RealtimeServer(spacecraft);
// var historyServer = new HistoryServer(spacecraft);
var staticServer = new StaticServer();
// var historyTflex = new HistoryServerTflex(tflex);
// var realtimeTflex =  new RealtimeServerTflex(tflex);

// app.use('/realtime', realtimeServer);
// app.use('/history', historyServer);
app.use('/', staticServer);
// app.use('/realtimeTflex', realtimeTflex);
// app.use('/historyTflex', historyTflex);

var port = process.env.PORT || 8080

app.listen(port, function () {
    console.log('Open MCT hosted at http://localhost:' + port);
    console.log('History hosted at http://localhost:' + port + '/history');
    console.log('Realtime hosted at ws://localhost:' + port + '/realtime');
});
