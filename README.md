# nodejs-gearman-c-binded [![Build Status](https://travis-ci.org/allevo/nodejs-gearman-c-binded.svg)](https://travis-ci.org/allevo/nodejs-gearman-c-binded)
An gearman client library implementation binded in C

This implementation is based on native libgearman library implemented in c (http://gearman.info/libgearman/index.html).

## Client

Now, this library implements only background job.
```javascript
  var GearmanClient = require('nodejs-gearman-c-binded').GearmanClient; 
  client = new GearmanClient();
  client.addServer('127.0.0.1', 4730);
  // unique may be null
  var job = client.doBackground('queue', 'data', 'unique', function(err) {
    if(err) {
      return console.log('Error:', err.message, 'with code', err.code);
    }
    console.log('Successful', job.handle());
    console.log('Successful', job.returnCode());
  });
```
Always add a server through `addServer` method.

`doBackground` method returns a job instance used later to retreive the handle and the return code. The callback is always called when gearman receives the job successfully or not. If some errors are occurred, the callback has one parameter, the error.


After submit a job you can give its status back using `getStatus` client method.
```javascript
var job = client.doBackground('queue', 'data', 'unique', function(err) {
  client.getStatus(job.handle(), function(err, result) {
    console.log(err); // should be undefined
    console.log(result.is_known, result.is_running, result.numerator, result.denominator);
  });
});
```
See tests for more explainations.

## Other functions

The libgearman library has many helper functions:
```javascript
  var nodejsGearmanCBinded = require('nodejs-gearman-c-binded');
  
  nodejsGearmanCBinded.strerror(nodejsGearmanCBinded.GEARMAN_SUCCESS); // 'GEARMAN_SUCCESS'
  nodejsGearmanCBinded.strerror(nodejsGearmanCBinded.GEARMAN_TIMEOUT); // 'GEARMAN_TIMEOUT'
  
  nodejsGearmanCBinded.gearmanSuccess(nodejsGearmanCBinded.GEARMAN_SUCCESS); // true
  nodejsGearmanCBinded.gearmanSuccess(nodejsGearmanCBinded.GEARMAN_TIMEOUT); // false
  
  nodejsGearmanCBinded.gearmanFailed(nodejsGearmanCBinded.GEARMAN_SUCCESS); // false
  nodejsGearmanCBinded.gearmanFailed(nodejsGearmanCBinded.GEARMAN_TIMEOUT); // true
  
  console.log(nodejsGearmanCBinded.LIBGEARMAN_VERSION_STRING); // your libgearman version string
  console.log(nodejsGearmanCBinded.LIBGEARMAN_VERSION_HEX); // your libgearman version hex
```

## Why
 - There're no gearman library for node that uses the native library. Each pure javascript implementation can have some problems or bug. This (I hope) not.
 - I would like to understand how to build a nodejs addons.

## Contributing
If you want contribute, you are welcome.
 - No coffescript are admitted.
 - All tests must be pass.
 - Use gearman always during the test to make your test as realistic as possible.
 - You contribution is very important!

All tests are simple without mocking object and using a real gearman server. Each test should be like your code.
