## 使用 VS Code 和 WSL 搭建 C++ 开发环境

本文整理自 Visual Studio Code 官方说明文档 [Using C++ and WSL in VS Code](https://code.visualstudio.com/docs/cpp/config-wsl)，对文档内容做了一些简化。

> 在本教程中，将配置 Visual Studio Code 以在 Linux 的 Windows 子系统（WSL） Ubuntu 上使用 GCC C++编译器（g++） 和 GDB 调试器。GCC 代表 GNU 编译器集合；GDB 是 GNU 调试器。WSL 是 Windows 中的 Linux 环境，它可以直接在计算机硬件上运行，而不是在虚拟机中运行。

### 前提条件

- 安装 WSL，[官方文档](https://docs.microsoft.com/zh-cn/windows/wsl/install-win10)
- 安装 VS Code，[官方下载地址](https://code.visualstudio.com/Download)

### WSL 配置

根据官方文档安装之后进入 WSL，首先安装编译和调试工具

> 需要更换镜像请参阅 [更换Ubuntu镜像源](https://yingming006.cn/change-ubuntu-repository-mirror-sources/)

```bash
sudo apt-get update
sudo apt-get install build-essential gdb
```

安装之后确认是否安装成功

```bash
whereis g++
whereis gdb
```

![image-20210225100530080](https://i.loli.net/2021/02/25/nSlRcFQD65pvHCf.png)

### 通过 WSL 启动 VS Code

新建一个项目文件夹，进入文件夹中，通过 `code .` 命令启动 WSL

```bash
mkdir helloworld
cd helloworld
code .
```

> 在这一步通常会自动进行安装 [Remote - WSL](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-wsl)，也可以在 VS Code 手动安装

### 配置 VS Code

安装  [Microsoft C/C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) 

### 新建 cpp 文件

```c++ helloworld.cpp
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
   vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};

   for (const string& word : msg)
   {
      cout << word << " ";
   }
   cout << endl;
}
```

### 编译 cpp 文件

1. 在菜单栏中，选择 `Terminal` > `Configure Default Build Task`，在弹出下拉栏中选择 `g ++ build active file`

    之后就会在当前根目录中生成 `.vscode/tasks.json` 文件

2. 使用快捷键 `Ctrl + Shift + B` 编译文件

    编译任务开始后，可以在终端界面中看到输出信息

![image-20210225103331854](https://i.loli.net/2021/02/25/8eUoBvS6LK57FI3.png)

3. 在终端执行文件：`./helloworld`

### 调试 cpp 文件

1. 在菜单栏中，选择 `Run` > `Add Configuration...`，在弹出下拉栏中选择 `C++ (GDB/LLDB)`，然后选择 `g++ build and debug active file`

    之后会在当前根目录下生成 `.vscode/launch.json`

2. 打开 `helloworld.cpp`，选择断点，使用快捷键 `F5` 或者在菜单栏选择 `Run` > `Start Debugging`



一些注意事项

1. VS Code 中自动补全使用的是 `Tab` 键。