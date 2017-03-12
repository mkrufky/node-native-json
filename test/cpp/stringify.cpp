/*********************************************************************
 * Native JSON for Node.js
 *
 * Copyright (c) 2017 Michael Ira Krufky
 *
 * MIT License <https://github.com/mkrufky/node-native-json/blob/master/LICENSE.md>
 ********************************************************************/

#include <native-json.h>

NAN_METHOD(Stringify) {
  info.GetReturnValue().Set(Native::JSON::Stringify(info[0]->ToObject()));
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target
    , Nan::New<v8::String>("stringify").ToLocalChecked()
    , Nan::New<v8::FunctionTemplate>(Stringify)->GetFunction()
  );
}

NODE_MODULE(stringify, Init)
