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
  static inline
  Nan::MaybeLocal<v8::Value> Parse(v8::Local<v8::String> jsonString) {
    Nan::EscapableHandleScope scope;

#if NATIVE_JSON_H_NEED_PARSE
    return scope.Escape(instance().parse(jsonString));
#else
#if (NODE_MAJOR_VERSION >= 7)
    Nan::MaybeLocal<v8::Value> result =
      v8::JSON::Parse(Nan::GetCurrentContext(), jsonString);

    if (result.IsEmpty()) return v8::Local<v8::Value>();
    return scope.Escape(result.ToLocalChecked());
#else
    return scope.Escape(v8::JSON::Parse(jsonString));
#endif
#endif
  }

  static inline
  Nan::MaybeLocal<v8::String> Stringify(v8::Local<v8::Object> jsonObject) {
    Nan::EscapableHandleScope scope;

    Nan::MaybeLocal<v8::String> result =
#if NATIVE_JSON_H_NEED_STRINGIFY
      Nan::To<v8::String>(instance().stringify(jsonObject));
#else
      v8::JSON::Stringify(Nan::GetCurrentContext(), jsonObject);
#endif
    if (result.IsEmpty()) return v8::Local<v8::String>();
    return scope.Escape(result.ToLocalChecked());
  }

  static inline
  Nan::MaybeLocal<v8::String> Stringify(v8::Local<v8::Object> jsonObject,
    v8::Local<v8::String> gap) {
    Nan::EscapableHandleScope scope;

    Nan::MaybeLocal<v8::String> result =
#if NATIVE_JSON_H_NEED_STRINGIFY
      Nan::To<v8::String>(instance().stringify(jsonObject, gap));
#else
      v8::JSON::Stringify(Nan::GetCurrentContext(), jsonObject, gap);
#endif
    if (result.IsEmpty()) return v8::Local<v8::String>();
    return scope.Escape(result.ToLocalChecked());
  }

 private:
  NAN_DISALLOW_ASSIGN_COPY_MOVE(JSON)
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
    v8::Local<v8::Value> globalJSON = Nan::Get(
      Nan::GetCurrentContext()->Global(), Nan::New("JSON").ToLocalChecked()
    ).ToLocalChecked();

    if (globalJSON->IsObject()) {
#if NATIVE_JSON_H_NEED_PARSE
      v8::Local<v8::Value> parseMethod = Nan::Get(
        Nan::To<v8::Object>(globalJSON).ToLocalChecked(),
        Nan::New("parse").ToLocalChecked()
      ).ToLocalChecked();

      if (!parseMethod.IsEmpty() && parseMethod->IsFunction()) {
        m_cb_parse.Reset(v8::Local<v8::Function>::Cast(parseMethod));
      }
#endif

#if NATIVE_JSON_H_NEED_STRINGIFY
      v8::Local<v8::Value> stringifyMethod = Nan::Get(
        Nan::To<v8::Object>(globalJSON).ToLocalChecked(),
        Nan::New("stringify").ToLocalChecked()
      ).ToLocalChecked();

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

  inline v8::Local<v8::Value> stringify(v8::Local<v8::Value> arg,
    v8::Local<v8::String> gap) {
    v8::Local<v8::Value> argv[] = {
      arg,
      Nan::Null(),
      gap
    };
    return m_cb_stringify.Call(3, argv);
  }
#endif
};

} /* namespace Native */

#endif /* NATIVE_JSON_H_ */
