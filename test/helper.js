'use strict';

var child_process = require('child_process');


var gearmanProcess;
function startGearmanServer(done) {
  gearmanProcess = child_process.spawn('gearmand', ['--port', '4731', '--verbose', 'DEBUG']);
  var alreadyCalled = false;
  gearmanProcess.stdout.on('data', function(data) {
    if (!alreadyCalled) {
      alreadyCalled = true;
      return done();
    }
  });
  gearmanProcess.stderr.on('data', function(data) {
    if (!alreadyCalled) {
      alreadyCalled = true;
      return done();
    }
  });
}

function stopGearmanServer(done) {
  if (!gearmanProcess) {
    return done();
  }
  gearmanProcess.on('exit', function() {
    done();
  });
  gearmanProcess.kill('SIGKILL');
}

module.exports = {
  startGearmanServer: startGearmanServer,
  stopGearmanServer: stopGearmanServer,
};
