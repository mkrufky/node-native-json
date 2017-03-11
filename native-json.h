#ifndef __NATIVE_JSON_H__
#define __NATIVE_JSON_H__

#include <nan.h>

namespace Native {

class JSON {
public:
  static v8::Local<v8::Value> Parse(v8::Local<v8::Value> jsonString)
  {
    return JSON::instance().parse(jsonString);
  }

  static v8::Local<v8::Value> Stringify(v8::Local<v8::Value> jsonObject)
  {
    return JSON::instance().stringify(jsonObject);
  }

private:
  Nan::Callback m_cb_parse;
  Nan::Callback m_cb_stringify;

  static JSON& instance()
  {
    static JSON i;
    return i;
  }

  JSON()
  {
    v8::Local<v8::Value> globalJSON = Nan::GetCurrentContext()->Global()->Get(Nan::New("JSON").ToLocalChecked());

    if (globalJSON->IsObject()) {
      v8::Local<v8::Value> parseMethod = globalJSON->ToObject()->Get(Nan::New("parse").ToLocalChecked());

      if (!parseMethod.IsEmpty() && parseMethod->IsFunction()) {
        m_cb_parse.Reset(v8::Local<v8::Function>::Cast(parseMethod));
      }

      v8::Local<v8::Value> stringifyMethod = globalJSON->ToObject()->Get(Nan::New("stringify").ToLocalChecked());

      if (!stringifyMethod.IsEmpty() && stringifyMethod->IsFunction()) {
        m_cb_stringify.Reset(v8::Local<v8::Function>::Cast(stringifyMethod));
      }
    }
  }

  ~JSON()
  {
    m_cb_parse.Reset();
    m_cb_stringify.Reset();
  }

  v8::Local<v8::Value> parse(v8::Local<v8::Value> arg)
  {
    return m_cb_parse.Call(1, &arg);
  }

  v8::Local<v8::Value> stringify(v8::Local<v8::Value> arg)
  {
    return m_cb_stringify.Call(1, &arg);
  }

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

}

#endif /* __NATIVE_JSON_H__ */
