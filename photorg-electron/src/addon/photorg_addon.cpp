#include "../cpp/photorg.h"
#include <napi.h>

Napi::Boolean OrganizePhotos(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 4) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, false);
  }

  std::string source_folder = info[0].As<Napi::String>().Utf8Value();
  bool yearly = info[1].As<Napi::Boolean>().Value();
  bool monthly = info[2].As<Napi::Boolean>().Value();
  bool daily = info[3].As<Napi::Boolean>().Value();

  return Napi::Boolean::New(
      env, Photorg::organize_photos(source_folder, yearly, monthly, daily));
}

Napi::Array GetDirectoryTree(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return Napi::Array::New(env, 0);
  }

  std::string path = info[0].As<Napi::String>().Utf8Value();
  std::vector<std::string> tree = Photorg::get_directory_tree(path);

  Napi::Array result = Napi::Array::New(env, tree.size());
  for (size_t i = 0; i < tree.size(); i++) {
    result.Set(i, Napi::String::New(env, tree[i]));
  }

  return result;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "organizePhotos"),
              Napi::Function::New(env, OrganizePhotos));
  exports.Set(Napi::String::New(env, "getDirectoryTree"),
              Napi::Function::New(env, GetDirectoryTree));
  return exports;
}

NODE_API_MODULE(photorg, Init)
