'use strict';

var fromNative = require('./build/Release/gearmannodeCBinded');

console.log(fromNative.LIBGEARMAN_VERSION_STRING);

function GearmanClient() {
  this.tasks = [];
  this.wrapGearmanClient = new fromNative.WrapGearmanClient();
}

GearmanClient.prototype.doBackground = function(queue, data, unique, callback) {
  var task = new fromNative.BackgroundTask(queue, data, unique);
  this.wrapGearmanClient.doBackground(task, function() {
    var err;
    if (task.returnCode() !== fromNative.GEARMAN_SUCCESS) {
      err = new Error('Gearman error');
      err.code = task.returnCode();
      /*jshint camelcase: false */
      err.code_description = fromNative.strerror(task.returnCode());
    }
    callback(err);
  });
  return task;
};

GearmanClient.prototype.setDebug = function() {
  this.wrapGearmanClient.setDebug(true);
};

GearmanClient.prototype.addServer = function(host, port) {
  this.wrapGearmanClient.addServer(host, port);
};

GearmanClient.prototype.stop = function(callback) {
  this.wrapGearmanClient.stop(callback);
};

for(var k in fromNative) {
  module.exports[k] = fromNative[k];
}
module.exports.GearmanClient = GearmanClient;
