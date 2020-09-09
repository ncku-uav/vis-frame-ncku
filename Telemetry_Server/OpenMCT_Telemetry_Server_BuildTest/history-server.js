// web framework fore nodejs
var express = require('express');

function HistoryServer(telemetrySource, port) {
    
    server = express();
    //allow requesting code from any origin to access the resource
    server.use(function (req, res, next) {
        res.set('Access-Control-Allow-Origin', '*');
        next();
    });
    // handle telemetry requests
    server.get('/telemetry/:pointId', function (req, res) {
        var start = +req.query.start;
        var end = +req.query.end;
        var ids = req.params.pointId.split(',');
        // build and send response
        var response = ids.reduce(function (resp, id) {
            return resp.concat(telemetrySource.history[id].filter(function (p) {
                return p.timestamp > start && p.timestamp < end;
            }));
        }, []);
        res.status(200).json(response).end();
    });

    server.listen(port);
    console.log('History server now running at http://localhost:' + port);
}

module.exports = HistoryServer;

