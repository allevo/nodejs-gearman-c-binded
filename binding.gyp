{
  "targets": [
    {
      "target_name": "gearmannodeCBinded",
      "sources": [
      	"src/init.cc",
        "src/WrapGearmanClient.cc",
        "src/BackgroundTask.cc",
      	"src/functions.cc",
      	"src/Baton.cc",
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
