'use strict';

var childProcess = require('child_process');


var gearmanProcess;
function startGearmanServer(done) {
  gearmanProcess = childProcess.spawn('gearmand', ['--port', '4731', '--verbose', 'DEBUG', '--log-file', 'stderr']);
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
  var readProcess = childProcess.spawn('php', [__dirname + '/../read.php', queue]);
  var d = '';
  readProcess.stdout.on('data', function(data) {
    console.log('on data', data.toString());
    d += data.toString();
  });
  readProcess.on('exit', function() {
    console.log('on exit', arguments);
    try {
      var splitted = d.split('\n');
      var parsed = [];
      for(var i in splitted) {
        console.log(splitted[i]);
        parsed.push(JSON.parse(splitted[i]));
      }
      callback(null, parsed);
    } catch (e) {
      console.log(d);
      callback('error here', e)
    }
  });
}

module.exports = {
  startGearmanServer: startGearmanServer,
  stopGearmanServer: stopGearmanServer,
  readAllJobs: readAllJobs,
};
