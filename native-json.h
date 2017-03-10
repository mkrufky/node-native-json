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
  const char *m_objectName;

  static JSON& instance()
  {
    static JSON i;
    return i;
  }

  JSON()
   : m_objectName("JSON")
  {
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    m_persistent.Reset(obj);

    v8::Local<v8::Value> objectName = Nan::New(m_objectName).ToLocalChecked();
    v8::Local<v8::Value> globalJSON = Nan::GetCurrentContext()->Global()->Get(objectName);

    if (globalJSON->IsObject()) {
      obj->Set(objectName, globalJSON);
    }
  }

  ~JSON()
  {
    m_persistent.Reset();
  }

  v8::Local<v8::Value> getMethod(v8::Local<v8::Object> &obj, const char *method)
  {
    v8::Local<v8::Object> persistent = Nan::New(m_persistent);
    v8::Local<v8::Value> methodName = Nan::New(method).ToLocalChecked();

    if (!persistent->Has(methodName))
    {
      v8::Local<v8::Value> thisMethod = obj->Get(methodName);

      if (thisMethod.IsEmpty() || !thisMethod->IsFunction()) {
        return Nan::Undefined();
      }

      persistent->Set(methodName, thisMethod);

      return thisMethod;
    }
    return persistent->Get(methodName);
  }

  v8::Local<v8::Value> call(const char *method, v8::Local<v8::Value> arg)
  {
    v8::Local<v8::Object> json = Nan::New(m_persistent)->Get(Nan::New(m_objectName).ToLocalChecked())->ToObject();

    v8::Local<v8::Function> methodFunction = v8::Local<v8::Function>::Cast(getMethod(json, method));

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
