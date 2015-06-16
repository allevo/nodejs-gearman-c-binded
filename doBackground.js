'use strict';


var g = require('./index');
var client = new g.GearmanClient();
client.setDebug(true);
client.addServer('127.0.0.1', 4730);

client.doJobBackground('queue', 'data', 'unique', function() {
  console.log('submited', arguments);
});

setTimeout(function() {
  console.log('AAA');
}, 100);
