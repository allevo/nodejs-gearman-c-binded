{
  "targets": [
    {
      "target_name": "gearmannodeCBinded",
      "sources": [ "src/init.cc", "src/GearmanClient.cc", "src/MultipleTask.cc" ],
      "link_settings": {
          "libraries": [
              "-lgearman"
          ]
      },
      "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
