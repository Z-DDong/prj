const hello = require('./build/Release/hello.node');

console.log(hello.hello()); // 输出

// for Initialize2 第二种导出方式可以这么调用
// console.log(hello()); // Hello World!

//exports.sayHello=function(){
//    return "Hello!"
//}


//module.exports.hello = () => 'world';
