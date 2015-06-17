'use strict';

var child_process = require('child_process');


var gearmanProcess;
function startGearmanServer(done) {
  gearmanProcess = child_process.spawn('gearmand', ['--port', '4731', '--verbose', 'DEBUG', '--log-file', 'stderr']);
  var alreadyCalled = false;
  function cb(context, data) {
    if ('stderr' === context) {
      // console.log(data.toString());
    }
    if (!alreadyCalled) {
      if (context === 'stderr' && data.toString().match(/replaying queue: end/)) {
        alreadyCalled = true;
        setTimeout(done, 0);
      }
      if (context === 'exit') {
        alreadyCalled = true;
        setTimeout(done.bind(null, new Error('gearman exited too early' + JSON.stringify(arguments))), 0);
      }
    }
  }
  gearmanProcess.stderr.on('data', cb.bind(null, 'stderr'));
  gearmanProcess.on('exit', cb.bind(null, 'exit'));
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

function readAllJobs(queue, callback) {
  var readProcess = child_process.spawn('php', [__dirname + '/../read.php', queue]);
  var d = '';
  readProcess.stdout.on('data', function(data) {
    d += data.toString();
  });
  readProcess.on('exit', function() {
    callback(null, d.split('\n').map(JSON.parse.bind(JSON)));
  });
}

module.exports = {
  startGearmanServer: startGearmanServer,
  stopGearmanServer: stopGearmanServer,
  readAllJobs: readAllJobs,
};
