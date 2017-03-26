/*********************************************************************
 * Native JSON for Node.js
 *
 * Copyright (c) 2017 Michael Ira Krufky
 *
 * MIT License <https://github.com/mkrufky/node-native-json/blob/master/LICENSE.md>
 ********************************************************************/

#include <native-json.h>

NAN_METHOD(Parse) {
  Nan::MaybeLocal<v8::String> inp = Nan::To<v8::String>(info[0]);

  if (inp.IsEmpty()) {
    info.GetReturnValue().Set(Nan::Undefined());
  } else {
    Nan::MaybeLocal<v8::Value> result = Native::JSON::Parse(
      inp.ToLocalChecked()
    );

    if (result.IsEmpty()) {
      info.GetReturnValue().Set(Nan::Undefined());
    } else {
      info.GetReturnValue().Set(result.ToLocalChecked());
    }
  }
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target
    , Nan::New<v8::String>("parse").ToLocalChecked()
    , Nan::New<v8::FunctionTemplate>(Parse)->GetFunction()
  );
}

NODE_MODULE(parse, Init)
