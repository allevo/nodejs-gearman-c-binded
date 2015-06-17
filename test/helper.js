'use strict';

var child_process = require('child_process');


var gearmanProcess;
function startGearmanServer(done) {
  gearmanProcess = child_process.spawn('gearmand', ['--port', '4731', '--verbose', 'DEBUG', '--log-file', 'stderr']);
  var alreadyCalled = false;
  function cb(context, data) {
    if (~['stdout', 'stderr'].indexOf(context)) {
      console.log(data.toString());
    }
    if (!alreadyCalled) {
      alreadyCalled = true;
      return done(context === 'exit' ? arguments : undefined);
    }
  }
  gearmanProcess.stdout.on('data', cb.bind(null, 'stdout'));
  gearmanProcess.stderr.on('data', cb.bind(null, 'stderr'));
  gearmanProcess.on('exit', cb.bind(null, 'exit'));
}

function stopGearmanServer(done) {
  if (!gearmanProcess) {
    return done();
  }
  gearmanProcess.on('exit', function() {
    console.log(arguments);
    done();
  });
  gearmanProcess.kill('SIGKILL');
}

module.exports = {
  startGearmanServer: startGearmanServer,
  stopGearmanServer: stopGearmanServer,
};
