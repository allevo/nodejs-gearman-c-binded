'use strict';

var assert = require('assert');

var GearmanClient = require('../index').GearmanClient;

describe('client', function () {
  describe('doBackground', function() {
    it('should store the data on gearman', function (done) {
      var client = new GearmanClient();
      client.addServer('127.0.0.1', 4730);
      client.doJobBackground('queue', 'data', 'unique', function(status, handler) {
        assert.equal(0, status);
        assert.equal(String, handler.constructor);
        done();
      });
    });
  });
});