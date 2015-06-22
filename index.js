'use strict';

var fromNative = require('./build/Release/gearmannodeCBinded');

function GearmanClient() {
  this.tasks = [];
  this.wrapGearmanClient = new fromNative.WrapGearmanClient();
}

GearmanClient.prototype.doBackground = function(queue, data, unique, callback) {
  var task = this.queueTaskBackground(queue, data, unique);
  this.execute(callback);
  return task;
}

GearmanClient.prototype.queueTaskBackground = function(queue, data, unique) {
  var task = new fromNative.GearmanTask(queue, data, unique);
  this.tasks.push(task);
  return task;
};

GearmanClient.prototype.addServer = function(host, port) {
  this.wrapGearmanClient.addServer(host, port);
};

GearmanClient.prototype.execute = function(callback) {
  var self = this;
  console.log(this.task);
  this.wrapGearmanClient._execute(this.tasks, function() {
    callback(self.tasks);
  });
};

for(var k in fromNative) {
  module.exports[k] = fromNative[k];
}
module.exports.GearmanClient = GearmanClient;
