# FLA_project

这是南京大学2024Fall形式语言与自动机课程的项目作业.

## 编译本项目
本框架依赖于cmake进行编译，需安装3.13.0以上版本的cmake方可使用。

本框架的使用方法为：

1. 将所有`.h` 以及`.cpp`文件置于`/fla-project`文件夹下，或者将该框架的`CMakeLists.txt`复制至你的工程根目录
2. 在工程根目录处，使用指令`cmake -B build`
3. 在工程根目录处，使用指令`cd ./build; make`

## 如何使用
你需要准备一个`.pda` 或者`.tm`文件,即给定下推自动机或图灵机.
例如: 
```
$ fla --help
usage: fla [-h|--help] <pda> <input>
fla [-v|--verbose] [-h|--help] <tm> <input>
$ fla anbn.pda aaabbb
true
```
在指定了 `-v|--verbose` 参数时,程序会给出详细的运行步骤.

