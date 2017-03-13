/*********************************************************************
 * Native JSON for Node.js
 *
 * Copyright (c) 2017 Michael Ira Krufky
 *
 * MIT License <https://github.com/mkrufky/node-native-json/blob/master/LICENSE.md>
 ********************************************************************/

#include <native-json.h>
#include <string>

NAN_METHOD(Stringify) {
  if (3 == info.Length()) {
    if (info[2]->IsNumber()) {
      int len = info[2]->IntegerValue();
      len = (len > 10) ? 10 : len;
      len = (len < 0) ? 0 : len;
      v8::Local<v8::String> gap =
        Nan::New<v8::String>(std::string(len, ' ')).ToLocalChecked();
      info.GetReturnValue().Set(
        Native::JSON::Stringify(info[0]->ToObject(), gap)
      );
    } else if (info[2]->IsString()) {
      info.GetReturnValue().Set(
        Native::JSON::Stringify(info[0]->ToObject(), info[2]->ToString())
      );
    } else {
      info.GetReturnValue().Set(Native::JSON::Stringify(info[0]->ToObject()));
    }
  } else {
    info.GetReturnValue().Set(Native::JSON::Stringify(info[0]->ToObject()));
  }
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target
    , Nan::New<v8::String>("stringify").ToLocalChecked()
    , Nan::New<v8::FunctionTemplate>(Stringify)->GetFunction()
  );
}

NODE_MODULE(stringify, Init)
