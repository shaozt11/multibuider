#include <iostream>
#include <windows.h>
using namespace std;

// ANSI 创建空标记文件
bool CreateCheckFileA(const char* szPath)
{
    HANDLE hFile = CreateFileA(
        szPath,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if(hFile == INVALID_HANDLE_VALUE)
        return false;
    CloseHandle(hFile);
    return true;
}

int main()
{
    // 这里写你的 GCC / NASM 编译命令
    system("gcc kernel.c -o kernel.o");

    // 编译完成，生成校验标记文件（对应CheckList里的路径）
    CreateCheckFileA("kernel.check");

    return 0;
}