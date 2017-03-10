#ifndef __NATIVE_JSON_H__
#define __NATIVE_JSON_H__

#include <nan.h>

namespace Native {

class JSON {
public:
  static inline v8::Local<v8::Value> Parse(v8::Local<v8::Value> jsonString)
  {
    return JSON::instance().call("parse", jsonString);
  }

  static inline v8::Local<v8::Value> Stringify(v8::Local<v8::Value> jsonObject)
  {
    return JSON::instance().call("stringify", jsonObject);
  }

private:
  Nan::Persistent<v8::Object> m_persistent;

  static JSON& instance()
  {
    static JSON i;
    return i;
  }

  JSON()
  {
    v8::Local<v8::Value> globalJSON = Nan::GetCurrentContext()->Global()->Get(Nan::New("JSON").ToLocalChecked());

    if (globalJSON->IsObject()) {
      m_persistent.Reset(globalJSON->ToObject());
    }
  }

  ~JSON()
  {
    m_persistent.Reset();
  }

  v8::Local<v8::Value> call(const char *method, v8::Local<v8::Value> arg)
  {
    v8::Local<v8::Object> json = Nan::New(m_persistent);

    v8::Local<v8::Value> thisMethod = json->Get(Nan::New(method).ToLocalChecked());

    if (thisMethod.IsEmpty() || !thisMethod->IsFunction()) {
      return Nan::Undefined();
    }

    v8::Local<v8::Function> methodFunction = v8::Local<v8::Function>::Cast(thisMethod);

    return methodFunction->Call(json, 1, &arg);
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
