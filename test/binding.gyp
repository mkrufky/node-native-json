{
  "target_defaults":
    {
        "cflags" : ["-Wall", "-Wextra", "-Wno-unused-parameter"],
        "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "<!(node -e \"require('..')\")"
        ]
    },
  "targets": [
    {
        "target_name" : "parse"
      , "sources"     : [ "cpp/parse.cpp" ]
    }
  , {
        "target_name" : "stringify"
      , "sources"     : [ "cpp/stringify.cpp" ]
    }
]}
