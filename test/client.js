'use strict';

var assert = require('assert');

var helper = require('./helper');
var cBinded = require('../index');
var GearmanClient = cBinded.GearmanClient;

describe('client', function () {
  beforeEach(helper.startGearmanServer);
  afterEach(helper.stopGearmanServer);

  describe('doBackground', function() {
    it('should store the data on gearman', function (done) {
      var client = new GearmanClient();
      client.addServer('127.0.0.1', 4731);
      client.doBackground('queue', 'data', 'unique', function(tasks) {
        assert.equal(1, tasks.length);
        assert.equal(tasks[0].ret, cBinded.GEARMAN_SUCCESS);
        assert.equal(tasks[0].unique, 'unique');

        helper.readAllJobs('queue', function(err, data) {
          assert.ifError(err);

          assert.equal(1, data.length);
          assert.equal(tasks[0].handle, data[0].handle);
          assert.equal('data', data[0].workload);
          assert.equal('queue', data[0].func);
          assert.equal('unique', data[0].unique);
          assert.equal(4, Object.keys(data[0]).length);

          done();
        });
      });
    });

    it('should store the data on gearman with different data', function (done) {
      var client = new GearmanClient();
      client.addServer('127.0.0.1', 4731);
      client.doBackground('foo', 'blablabla', 'unique2', function(tasks) {
        assert.equal(1, tasks.length);
        assert.equal(tasks[0].ret, cBinded.GEARMAN_SUCCESS);
        assert.equal(tasks[0].unique, 'unique2');

        helper.readAllJobs('foo', function(err, data) {
          assert.ifError(err);

          assert.equal(1, data.length);
          assert.equal(tasks[0].handle, data[0].handle);
          assert.equal('blablabla', data[0].workload);
          assert.equal('foo', data[0].func);
          assert.equal(tasks[0].unique, data[0].unique);
          assert.equal(4, Object.keys(data[0]).length);

          done();
        });
      });
    });

    xit('should store the data on gearman without unique', function (done) {
      var client = new GearmanClient();
      client.addServer('127.0.0.1', 4731);
      client.doBackground('foo', 'blablabla', null, function(status, handler) {
        assert.equal(cBinded.GEARMAN_SUCCESS, status);
        assert.equal(String, handler.constructor);

        helper.readAllJobs('foo', function(err, data) {
          assert.ifError(err);

          assert.equal(1, data.length);
          assert.equal(handler, data[0].handle);
          assert.equal('blablabla', data[0].workload);
          assert.equal('foo', data[0].func);
          // generated by gearman server
          assert.equal(36, data[0].unique.length);
          assert.equal(4, Object.keys(data[0]).length);

          done();
        });
      });
    });

    it('should return error if gearman is unreachable', function (done) {
      var client = new GearmanClient();
      client.addServer('127.0.0.1', 4731);

      helper.stopGearmanServer(function() {
        client.doBackground('queue', 'data', 'unique', function(tasks) {
          console.log(tasks);

          assert.equal(cBinded.GEARMAN_COULD_NOT_CONNECT, status);
          assert.equal(undefined, handler);

          done();
        });
      });
    });

    it.only('should return error if gearman is unreachable on second', function (done) {
      var client = new GearmanClient();
      client.addServer('127.0.0.1', 4731);

      client.doBackground('queue', 'data', 'unique', function(tasks) {
        assert.equal(1, tasks.length);
        assert.equal(cBinded.GEARMAN_SUCCESS, tasks[0].returnCode());

        helper.stopGearmanServer(function() {
          client.doBackground('queue', 'data', 'unique', function(tasks2) {
            assert.equal(cBinded.GEARMAN_LOST_CONNECTION, tasks2[0].returnCode());
            assert.equal(cBinded.GEARMAN_SUCCESS, tasks2[0].returnCode());

            done();
          });
        });
      });
    });
  });

  it('should store the added tasks', function (done) {
    var client = new GearmanClient();
    client.addServer('127.0.0.1', 4731);

    for(var i = 1; i <= 10; i++) {
      client.queueTaskBackground('queue', 'data' + i, 'unique' + i);
    }
    client.execute(function(tasks) {
      assert.equal(10, tasks.length);
      for(var i = 0; i < tasks.length; i++) {
        assert.ok(tasks[i].handle.match(/^H:[^:]+:\d+$/));
        assert.equal('unique' + (i + 1), tasks[i].unique);
      }

      helper.readAllJobs('queue', function(err, data) {
        assert.equal(10, data.length);

        for(var i =0; i < data.length; i++) {
          assert.equal(data[i].func, 'queue');
          assert.equal(data[i].handle, tasks[i].handle);
          assert.equal(data[i].unique, tasks[i].unique);
          assert.equal('data' + (i + 1), data[i].workload);
        }
        done();

      });
    });
  });
});
