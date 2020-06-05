var express = require('express');

function HistoryServerTflex(tflex) {
    var router = express.Router();

    router.get('/:pointId', function (req, res) {
        var start = +req.query.start;
        var end = +req.query.end;
        var ids = req.params.pointId.split(',');

        var response = ids.reduce(function (resp, id) {
            return resp.concat(tflex.history[id].filter(function (p) {
                return p.timestamp > start && p.timestamp < end;
            }));
        }, []);
        res.status(200).json(response).end();
    });

    return router;
}

module.exports = HistoryServerTflex;

