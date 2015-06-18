'use strict';

var assert = require('assert');

var helper = require('./helper');
var cBinded = require('../index');
var GearmanClient = cBinded.GearmanClient;

describe('client', function () {
  beforeEach(helper.startGearmanServer);
  afterEach(helper.stopGearmanServer);

  describe('doBackground', function() {
    it.only('should store the data on gearman', function (done) {
      var client = new GearmanClient();
      client.addServer('127.0.0.1', 4731);
      client.doJobBackground('queue', 'data', 'unique', function(status, handler) {
        assert.equal(cBinded.GEARMAN_SUCCESS, status);
        assert.equal(String, handler.constructor);

        helper.readAllJobs('queue', function(err, data) {
          assert.ifError(err);
          console.log(data);
          /*
          assert.equal(1, data.length);
          assert.equal(String, data[0].handler.constructor);
          assert.equal('data', data[0].workload);
          assert.equal('queue', data[0].func);
          assert.equal('unique', data[0].unique);
          assert.equal(4, Object.keys(data[0]).length);
          */
          done();
        });
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
