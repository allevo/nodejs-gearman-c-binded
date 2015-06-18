'use strict';

var childProcess = require('child_process');


var gearmanProcess;
function startGearmanServer(done) {
  gearmanProcess = childProcess.spawn('gearmand', ['--port', '4731', '--verbose', 'DEBUG', '--log-file', 'stderr']);
  var alreadyCalled = false;
  function cb(context, data) {
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
    gearmanProcess = null;
    done();
  });
  gearmanProcess.kill('SIGKILL');
}

function readAllJobs(queue, callback) {
  var readProcess = childProcess.spawn(__dirname + '/../read', [queue]);
  var d = '';
  readProcess.stdout.on('data', function(data) {
    d += data.toString();
  });
  readProcess.on('exit', function() {
    var parsed = [];
    try {
      var splitted = d.split('\n');
      splitted.pop();
      for(var i in splitted) {
        var parts = splitted[i].split('|');
        parsed.push({
          workload: parts[0],
          unique: parts[1],
          handle: parts[2],
          func: parts[3],
        });
      }
    } catch (e) {
      console.log(d);
      return callback(e);
    }
    callback(null, parsed);
  });
}

process.on('exit', function() {
  stopGearmanServer(function() {});
});

module.exports = {
  startGearmanServer: startGearmanServer,
  stopGearmanServer: stopGearmanServer,
  readAllJobs: readAllJobs,
};
