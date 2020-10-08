  //var http = require("http");
  //var server = http.createServer(function(req,res){
  //res.writeHead(200,{'Content-Type':'text/html'});
  //     res.end("hello user");
  //}); server.listen(3000);

var WebSocket = require('ws')

  var ws = new WebSocket('ws://localhost:8080/');

  ws.onmessage = function(event) {
    console.log('Count is: ' + event.data);
  };
