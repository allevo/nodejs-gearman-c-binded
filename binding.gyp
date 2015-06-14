{
  "targets": [
    {
      "target_name": "gearmannodeCBinded",
      "sources": [ "src/init.cc", "src/GearmanClient.cc", "src/GearmanTask.cc" ],
      'link_settings': {
          'libraries': [
              '-lgearman'
          ]
      }
    }
  ]
}
