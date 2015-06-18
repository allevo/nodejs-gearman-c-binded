'use strict';

var assert = require('assert');
var cBinded = require('../index');


describe('module', function () {
  describe('strerror', function () {
    it('should be a function', function () {
      assert.equal(Function, cBinded.strerror.constructor);
    });

    it('should return correct value', function () {
      assert.equal('GEARMAN_SUCCESS', cBinded.strerror(cBinded.GEARMAN_SUCCESS));
      assert.equal('GEARMAN_IO_WAIT', cBinded.strerror(cBinded.GEARMAN_IO_WAIT));
    });
  });

  describe('gearmanSuccess', function () {
    it('should be a function', function () {
      assert.equal(Function, cBinded.gearmanSuccess.constructor);
    });

    it('should return correct value', function () {
      assert.equal(true, cBinded.gearmanSuccess(cBinded.GEARMAN_SUCCESS));
      assert.equal(false, cBinded.gearmanSuccess(cBinded.GEARMAN_TIMEOUT));
    });
  });

  describe('gearmanFailed', function () {
    it('should be a function', function () {
      assert.equal(Function, cBinded.gearmanFailed.constructor);
    });

    it('should return correct value', function () {
      assert.equal(false, cBinded.gearmanFailed(cBinded.GEARMAN_SUCCESS));
      assert.equal(true, cBinded.gearmanFailed(cBinded.GEARMAN_TIMEOUT));
    });
  });

  describe('gearmanContinue', function () {
    it('should be a function', function () {
      assert.equal(Function, cBinded.gearmanContinue.constructor);
    });

    it('should return correct value', function () {
      assert.equal(false, cBinded.gearmanContinue(cBinded.GEARMAN_SUCCESS));
      assert.equal(false, cBinded.gearmanContinue(cBinded.GEARMAN_TIMEOUT));
    });
  });

  it('should export LIBGEARMAN_VERSION_STRING', function () {
    assert.ok(cBinded.LIBGEARMAN_VERSION_STRING.match(/\d+\.\d+(.\d+)?/));
  });

  it('should export LIBGEARMAN_VERSION_HEX', function () {
    assert.equal(Number, cBinded.LIBGEARMAN_VERSION_HEX.constructor);
  });

  it('should export GearmanClient', function () {
    assert.equal(Function, cBinded.GearmanClient.constructor);
  });

  it('should export MultipleTask', function () {
    assert.equal(Function, cBinded.MultipleTask.constructor);
  });

  it('should export all gearman constant', function () {
    var con = [
      'GEARMAN_SUCCESS',
      'GEARMAN_IO_WAIT',
      'GEARMAN_SHUTDOWN',
      'GEARMAN_SHUTDOWN_GRACEFUL',
      'GEARMAN_ERRNO',
      'GEARMAN_EVENT',
      'GEARMAN_TOO_MANY_ARGS',
      'GEARMAN_NO_ACTIVE_FDS',
      'GEARMAN_INVALID_MAGIC',
      'GEARMAN_INVALID_COMMAND',
      'GEARMAN_INVALID_PACKET',
      'GEARMAN_UNEXPECTED_PACKET',
      'GEARMAN_GETADDRINFO',
      'GEARMAN_NO_SERVERS',
      'GEARMAN_LOST_CONNECTION',
      'GEARMAN_MEMORY_ALLOCATION_FAILURE',
      'GEARMAN_JOB_EXISTS',
      'GEARMAN_JOB_QUEUE_FULL',
      'GEARMAN_SERVER_ERROR',
      'GEARMAN_WORK_ERROR',
      'GEARMAN_WORK_DATA',
      'GEARMAN_WORK_WARNING',
      'GEARMAN_WORK_STATUS',
      'GEARMAN_WORK_EXCEPTION',
      'GEARMAN_WORK_FAIL',
      'GEARMAN_NOT_CONNECTED',
      'GEARMAN_COULD_NOT_CONNECT',
      'GEARMAN_SEND_IN_PROGRESS',
      'GEARMAN_RECV_IN_PROGRESS',
      'GEARMAN_NOT_FLUSHING',
      'GEARMAN_DATA_TOO_LARGE',
      'GEARMAN_INVALID_FUNCTION_NAME',
      'GEARMAN_INVALID_WORKER_FUNCTION',
      'GEARMAN_NO_REGISTERED_FUNCTION',
      'GEARMAN_NO_REGISTERED_FUNCTIONS',
      'GEARMAN_NO_JOBS',
      'GEARMAN_ECHO_DATA_CORRUPTION',
      'GEARMAN_NEED_WORKLOAD_FN',
      'GEARMAN_PAUSE',
      'GEARMAN_UNKNOWN_STATE',
      'GEARMAN_PTHREAD',
      'GEARMAN_PIPE_EOF',
      'GEARMAN_QUEUE_ERROR',
      'GEARMAN_FLUSH_DATA',
      'GEARMAN_SEND_BUFFER_TOO_SMALL',
      'GEARMAN_IGNORE_PACKET',
      'GEARMAN_UNKNOWN_OPTION',
      'GEARMAN_TIMEOUT',
      'GEARMAN_ARGUMENT_TOO_LARGE',
      'GEARMAN_INVALID_ARGUMENT',
      'GEARMAN_IN_PROGRESS',
    ];
    for (var i in con) {
      assert.equal(i, cBinded[con[i]]);
    }
  });
});
