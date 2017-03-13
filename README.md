# native-json

Access the `v8::JSON` object methods `Parse` & `Stringify` from native c++ addons across all versions of node.js

[![NPM](https://nodei.co/npm/native-json.png?downloads=true&downloadRank=true)](https://nodei.co/npm/native-json/)

[![Build Status](https://api.travis-ci.org/mkrufky/node-native-json.svg?branch=master)](http://travis-ci.org/mkrufky/node-native-json)
[![Build status](https://ci.appveyor.com/api/projects/status/p5cgmixxpftd8hxa?svg=true)](https://ci.appveyor.com/project/mkrufky/node-native-json)

native-json is a header package for building c++ native addons for node.js that is meant to complement **[nan](https://github.com/nodejs/nan)** by adding an interface to the `v8::JSON` object's `Parse` and `Stringify` methods that can be relied upon regardless of the version of node.js that the addon is being built against.

<a name="usage"></a>

## Usage

Simply add **nan** and **native-json** as dependencies in the *package.json* of your Node addon:

``` bash
$ npm install --save nan
$ npm install --save native-json
```

Pull in the paths to **nan** and **native-json** in your *binding.gyp* so that you can use `#include <native-json.h>` in your *.cpp* files:

``` python
"include_dirs" : [
    "<!(node -e \"require('nan')\")",
    "<!(node -e \"require('native-json')\")"
]
```

This works like a `-I<path-to-native-json>` when compiling your addon.

<a name="api"></a>

## API

``` c++
Nan::MaybeLocal<v8::Value> Parse(v8::Local<v8::String> jsonString);

Nan::MaybeLocal<v8::String> Stringify(v8::Local<v8::Object> jsonObject);
Nan::MaybeLocal<v8::String> Stringify(v8::Local<v8::Object> jsonObject, v8::Local<v8::String> gap);
```

To access the javascript function `JSON.parse()` from your c++ code, call `Native::JSON::Parse()` where you would normally call `v8::JSON::Parse()`, which would otherwise not be available in versions of node.js older than 0.12.x

``` c++
v8::Local<v8::String> jsonString =
  Nan::New("{ \"JSON\": \"object\" }").ToLocalChecked();

v8::Local<v8::Value> parsedValue =
  Native::JSON::Parse(jsonString).ToLocalChecked();
```

To access the javascript function `JSON.stringify()` from your c++ code, call `Native::JSON::Stringify()` where you would normally call `v8::JSON::Stringify()`, which would otherwise not be available in versions of node.js older than 7.x

``` c++
v8::Local<v8::Object> object;

v8::Local<v8::String> stringified =
  Native::JSON::Stringify(object).ToLocalChecked();
```

<a name="tests"></a>

### Tests

To run the native-json tests do:

``` sh
npm install
npm run-script rebuild-tests
npm test
```

Or just:

``` sh
npm install
make test
```

## Licence &amp; copyright

Copyright (c) 2017 Michael Ira Krufky

native-json is licensed under an MIT license. All rights not explicitly granted in the MIT license are reserved. See the included LICENSE file for more details.
