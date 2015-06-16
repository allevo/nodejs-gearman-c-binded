{
  "targets": [
    {
      "target_name": "gearmannodeCBinded",
      "sources": [ "src/init.cc", "src/GearmanClient.cc" ],
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
