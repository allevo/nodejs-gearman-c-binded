{
  "targets": [
    {
      "target_name": "gearmannodeCBinded",
      "sources": [
      	"src/init.cc",
        "src/WrapGearmanClient.cc",
        "src/GearmanTask.cc",
      	"src/functions.cc"
      ],
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
