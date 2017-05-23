{
  "targets": [
    {
      "target_name": "naudiodon",
      "sources": [
        "src/naudiodon.cc",
        "src/GetDevices.cc",
        "src/AudioOutput.cc",
	"src/AudioInput.cc",
	"src/common.cc",
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")", "portaudio/include"
      ],
      "conditions" : [
        [
          'OS=="mac"', {
            'xcode_settings': {
              'GCC_ENABLE_CPP_RTTI': 'YES',
              'MACOSX_DEPLOYMENT_TARGET': '10.7',
              'OTHER_CPLUSPLUSFLAGS': [
                '-std=c++11',
                '-stdlib=libc++',
                '-fexceptions'
              ],
              'OTHER_LDFLAGS': [
                "-Wl,-rpath,<@(module_root_dir)/build/Release"
              ]
            },
            "link_settings": {
              "libraries": [
                "<@(module_root_dir)/build/Release/libportaudio.dylib"
              ]
            },
            "copies": [
              {
                "destination": "build/Release/",
                "files": [
                  "<!@(ls -1 portaudio/bin/libportaudio.dylib)"
                ]
              }
            ]
          },
        ],
        [
          'OS=="win"', {
            "configurations": {
              "Release": {
                "msvs_settings": {
                  "VCCLCompilerTool": {
                    "RuntimeTypeInfo": "true",
                    "ExceptionHandling": 1
                  }
                }
              }
            },
            'conditions': [
              ['target_arch=="x64"', {
                'libraries': [
                  '../portaudio/bin/portaudio_static_x64.lib'
                ],
              }, {
                'libraries': [
                  '../portaudio/bin/portaudio_static_x86.lib'
                ],
              }],
            ]
          },
        ],
        [
          'OS=="linux"', {
            "conditions": [
              ['target_arch=="arm"', {
                "link_settings": {
                  "libraries": [
                    "<@(module_root_dir)/build/Release/libportaudio.so.2"
                  ],
                  "ldflags": [
                    "-L<@(module_root_dir)/build/Release",
                    "-Wl,-rpath,<@(module_root_dir)/build/Release"
                  ]
                },
                "copies": [
                  {
                    "destination": "build/Release/",
                    "files": [
                      "<@(module_root_dir)/portaudio/bin_armhf/libportaudio.so.2"
                    ]
                  }
                ]
              },
              { # ia32 or x64
                "link_settings": {
                  "libraries": [
                    "<@(module_root_dir)/build/Release/libportaudio.so.2"
                  ],
                  "ldflags": [
                  "-L<@(module_root_dir)/build/Release",
                  "-Wl,-rpath,<@(module_root_dir)/build/Release"
                  ]
                },
                "copies": [
                  {
                    "destination": "build/Release/",
                    "files": [
                      "<@(module_root_dir)/portaudio/bin/libportaudio.so.2"
                    ]
                  }
                ]
              }]
            ]
          }
        ]
      ]
    }
  ]
}
