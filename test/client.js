'use strict';

var assert = require('assert');

var helper = require('./helper');
var cBinded = require('../index');
var GearmanClient = cBinded.GearmanClient;

var gearmanProcess;
describe('client', function (done) {
  beforeEach(helper.startGearmanServer);
  afterEach(helper.stopGearmanServer);
  describe('doBackground', function() {
    it('should store the data on gearman', function (done) {
      var client = new GearmanClient();
      client.addServer('127.0.0.1', 4731);
      client.doJobBackground('queue', 'data', 'unique', function(status, handler) {
        assert.equal(cBinded.GEARMAN_SUCCESS, status);
        assert.equal(String, handler.constructor);
        done();
      });
    });
  });

  describe('getMultipleTask', function () {
    it('should return a multiple task object', function () {
      var client = new GearmanClient();
      client.addServer('127.0.0.1', 4731);

      var mTask = client.getMultipleTask();

      assert.equal(cBinded.MultipleTask, mTask.constructor);
    });
  });
});
