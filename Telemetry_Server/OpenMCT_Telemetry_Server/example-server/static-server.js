var express = require('express');

function StaticServer(port) {
    var server = express();

    server.use('/', express.static(__dirname + '/..'));



    console.log('Open MCT hosted at http://localhost:' + port);

    server.listen(port);
}

module.exports = StaticServer; //exports module to read in with require
