'use strict';

var assert = require('assert');

var helper = require('./helper');
var index = require('../index');
var GearmanWorker = index.GearmanWorker;


var worker;
describe.only('worker', function () {
  beforeEach(helper.startGearmanServer);
  afterEach(helper.stopGearmanServer);

  beforeEach(function() {
    worker = new GearmanWorker();
    worker.addServer('127.0.0.1', 4731);
  });
  afterEach(function(done) {
    worker.stop(done);
  })

  it('foo', function(done) {

    worker.addFunction('queue', function() {
      console.log(arguments);
    });

    worker.start();

    helper.queueJob('queue', 'data', 'unique', function() {
    });
  });
});