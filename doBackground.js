'use strict';


var g = require('./index');

var client = new g.GearmanClient();
client.addServer('127.0.0.1', 4730);
var job = client.doooo(function() {
  // console.log(job);
  console.log('GGGGGGGGGGGGGGGGGGGGGG', job.handle());
  console.log(job.returnCode());
});
