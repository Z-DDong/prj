// hello.cc
#include <node.h>
#include "atlstr.h"
namespace demo {
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
// 动态调用DLL库
const char* DynamicUse()
{
    // 运行时加载DLL库
    HMODULE module = LoadLibraryA("MyDLL01.dll");
    if (module == NULL)
    {
        printf("加载MyDLL01.dll动态库失败\n");
        return "";
    }
    // 定义函数指针类型
    typedef const char*(*Func)();
    Func SaySuccess;
    // 导出函数地址
    SaySuccess = (Func)GetProcAddress(module, "SaySuccessStr");     // GetProcAddress返回指向的函数名的函数地址

    const char* str = SaySuccess();
    return str;
    //printf("%s", str);

}
//内部方法接口
void Method(const FunctionCallbackInfo<Value>& args) {
    const char* str=DynamicUse();
    Isolate* isolate = args.GetIsolate();
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, str).ToLocalChecked());
}

//给传入的目标添加Method()方法
void Initialize(Local<Object> exports) {
  NODE_SET_METHOD(exports, "hello", Method);
}
//调用NODE_MODULE()将注册方法定义到内存中
NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

}  // namespace demo

/*
编译C++：
需要node-gyp命令进行编译
1.node-gyp要和node版本一致
2.安装命令：npm install -g node-gyp
3.生成Makefile：node-gyp configure
4.编译：node-gyp build
(或者合并生成和编译：node-gyp configure build)

成功之后：当前目录内会有build\Release\hello.node文件
node文件其实就是动态库，linux下是so，windows下是dll
node.js v8引擎会使用dlopen的方式加载这个动态库。

node.js 调用：
{
    const hello = require('./build/Release/hello.node');
    console.log(hello.hello()); // 输出

}
调试：
node index.js
*/
