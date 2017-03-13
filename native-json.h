/*********************************************************************
 * Native JSON for Node.js
 *
 * Copyright (c) 2017 Michael Ira Krufky <https://github.com/mkrufky>
 *
 * MIT License <https://github.com/mkrufky/node-native-json/blob/master/LICENSE.md>
 *
 * See https://github.com/mkrufky/node-native-json for the latest update to this file
 ********************************************************************/


#ifndef NATIVE_JSON_H_
#define NATIVE_JSON_H_

#include <nan.h>

#if defined(NODE_MAJOR_VERSION) && defined(NODE_MINOR_VERSION)

#if ((NODE_MAJOR_VERSION == 0) && (NODE_MINOR_VERSION < 12))
#define NATIVE_JSON_H_NEED_PARSE 1
#else
#define NATIVE_JSON_H_NEED_PARSE 0
#endif

#if (NODE_MAJOR_VERSION >= 7)
#define NATIVE_JSON_H_NEED_STRINGIFY 0
#else
#define NATIVE_JSON_H_NEED_STRINGIFY 1
#endif

#else
#define NATIVE_JSON_H_NEED_PARSE 1
#define NATIVE_JSON_H_NEED_STRINGIFY 1
#endif

namespace Native {

class JSON {
 public:
  static
  inline v8::Local<v8::Value> Parse(v8::Local<v8::String> jsonString) {
#if NATIVE_JSON_H_NEED_PARSE
    return instance().parse(jsonString);
#else
#if (NODE_MAJOR_VERSION >= 7)
    return v8::JSON::Parse(Nan::GetCurrentContext(),
      jsonString).ToLocalChecked();
#else
    return v8::JSON::Parse(jsonString);
#endif
#endif
  }

  static
  inline v8::Local<v8::String> Stringify(v8::Local<v8::Object> jsonObject) {
#if NATIVE_JSON_H_NEED_STRINGIFY
    return instance().stringify(jsonObject)->ToString();
#else
    return v8::JSON::Stringify(Nan::GetCurrentContext(),
      jsonObject).ToLocalChecked();
#endif
  }

 private:
#if NATIVE_JSON_H_NEED_PARSE
  Nan::Callback m_cb_parse;
#endif
#if NATIVE_JSON_H_NEED_STRINGIFY
  Nan::Callback m_cb_stringify;
#endif

#if (NATIVE_JSON_H_NEED_PARSE + NATIVE_JSON_H_NEED_STRINGIFY)
  static JSON& instance() {
    static JSON i;
    return i;
  }
#endif

  JSON() {
#if (NATIVE_JSON_H_NEED_PARSE + NATIVE_JSON_H_NEED_STRINGIFY)
    v8::Local<v8::Value> globalJSON =
      Nan::GetCurrentContext()->Global()->Get(
        Nan::New("JSON").ToLocalChecked()
      );

    if (globalJSON->IsObject()) {
#if NATIVE_JSON_H_NEED_PARSE
      v8::Local<v8::Value> parseMethod =
        globalJSON->ToObject()->Get(Nan::New("parse").ToLocalChecked());

      if (!parseMethod.IsEmpty() && parseMethod->IsFunction()) {
        m_cb_parse.Reset(v8::Local<v8::Function>::Cast(parseMethod));
      }
#endif

#if NATIVE_JSON_H_NEED_STRINGIFY
      v8::Local<v8::Value> stringifyMethod =
        globalJSON->ToObject()->Get(Nan::New("stringify").ToLocalChecked());

      if (!stringifyMethod.IsEmpty() && stringifyMethod->IsFunction()) {
        m_cb_stringify.Reset(v8::Local<v8::Function>::Cast(stringifyMethod));
      }
#endif
    }
#endif
  }

  ~JSON() {
#if NATIVE_JSON_H_NEED_PARSE
    m_cb_parse.Reset();
#endif
#if NATIVE_JSON_H_NEED_STRINGIFY
    m_cb_stringify.Reset();
#endif
  }

#if NATIVE_JSON_H_NEED_PARSE
  inline v8::Local<v8::Value> parse(v8::Local<v8::Value> arg) {
    return m_cb_parse.Call(1, &arg);
  }
#endif

#if NATIVE_JSON_H_NEED_STRINGIFY
  inline v8::Local<v8::Value> stringify(v8::Local<v8::Value> arg) {
    return m_cb_stringify.Call(1, &arg);
  }
#endif

#if __cplusplus <= 199711L

 private:
  JSON(JSON const&);
  void operator=(JSON const&);
#else

 public:
  JSON(JSON const&)           = delete;
  void operator=(JSON const&) = delete;
#endif
};

} /* namespace Native */

#endif /* NATIVE_JSON_H_ */
