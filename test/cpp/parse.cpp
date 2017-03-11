#include <native-json.h>

NAN_METHOD(Parse) {
  info.GetReturnValue().Set(Native::JSON::Parse(info[0]->ToString()));
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target
    , Nan::New<v8::String>("parse").ToLocalChecked()
    , Nan::New<v8::FunctionTemplate>(Parse)->GetFunction()
  );
}

NODE_MODULE(parse, Init)
