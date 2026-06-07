# MultiBuilder - XJ380 多任务构建器

一个为 XJ380 操作系统开发的多进程并行构建调度工具，用于自动化管理多个编译任务的启动、等待和收尾。

## 功能特性

- 🚀 **多进程并行启动** - 一次性启动多个任务进程
- ⏱️ **智能等待校验** - 通过检测文件生成来判断任务完成
- 🧹 **自动清理收尾** - 完成后自动清理标记文件并执行后续命令
- 💻 **Windows 原生** - 专为 Windows 开发，使用 Win32 API

## 工作原理

### 1. 任务启动 (BeginTask)
读取 `StartList.ini` 配置文件，逐行使用 `start` 命令后台启动所有任务程序。

### 2. 等待校验 (CheckComplete)
读取 `CheckList.ini` 配置文件，持续轮询检查标记文件是否生成，无序实时播报完成状态。

### 3. 收尾清理 (AfterTask)
删除所有校验标记文件，读取 `After.ini` 执行后续收尾命令。

## 配置文件

### StartList.ini
每行一个可执行文件路径，用于启动任务：
```
task1.exe
task2.exe
task3.exe
```

### CheckList.ini
每行一个标记文件路径，用于判断任务是否完成：
```
task1.check
task2.check
task3.check
```

### After.ini
每行一个命令，在所有任务完成后执行：
```
echo All tasks completed!
copy output/* ./result/
```

## 编译方法

使用 MinGW-w64 编译：
```bash
x86_64-w64-mingw32-g++ -o multibuider.exe main.cpp
```

## 使用环境

- **操作系统**: Windows 任意版本 (XP/7/8/10/11)
- **编译工具**: MinGW-w64
- **开发语言**: C++
- **推荐搭配**: WSL2 (Ubuntu) 用于交叉编译

## 相关项目

- **XJ380 OS** - 自研 x86_64 UEFI 操作系统
- **OpenXSKernel** - UEFI 64位系统引导程序
- **StardustUI** - 跨平台 C++ UI 框架
- **BridgeEngine** - 跨平台 2D 引擎

## 开发说明

> 所有思路由初二学生独立思考，所有代码由豆包完成
