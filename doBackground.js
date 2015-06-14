'use strict';


var g = require('./index');
var client = new g.GearmanClient();

var r;
r = client.addServer('127.0.0.1', 4730);
// console.log(r);


// client.start();

// r = client.doJobBackgroundSync();


var n = 0;
var id = setInterval(function() {
  n++;
  client.doJobBackground(function(status, handler) {
    console.log("AAAAAAAA SUBMITTED", status === g.GearmanClient.GEARMAN_SUCCESS, handler);
  });

}, 50);
