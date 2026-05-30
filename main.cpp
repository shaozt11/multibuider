#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

// 全局存储列表（ANSI字符串）
vector<string> g_TaskExeList;      // StartList.ini 任务程序
vector<string> g_CheckFileList;    // CheckList.ini 校验文件
vector<string> g_AfterCmdList;     // After.ini 收尾命令
vector<bool>   g_FinishState;      // 对应校验文件完成状态

// ANSI 判断文件是否存在
bool IsFileExistA(const char* szPath)
{
    DWORD dwAttr = GetFileAttributesA(szPath);
    if(dwAttr == INVALID_FILE_ATTRIBUTES)
        return false;
    if(dwAttr & FILE_ATTRIBUTE_DIRECTORY)
        return false;
    return true;
}

// ANSI 删除单个文件
void DeleteSingleFileA(const char* szPath)
{
    DeleteFileA(szPath);
}

// 1. 启动多进程任务函数 BeginTask
void BeginTask()
{
    cout << "=====================================" << endl;
    cout << "即将开始多进程并行任务调度" << endl;
    cout << "=====================================" << endl;

    ifstream fin("StartList.ini", ios::in);
    if(!fin.is_open())
    {
        cout << "错误：未找到 StartList.ini 配置文件！" << endl;
        return;
    }

    string line;
    while(getline(fin, line))
    {
        if(line.empty()) continue;
        g_TaskExeList.push_back(line);

        // CMD start 后台并行启动程序（ANSI）
        string cmd = "start \"\" \"" + line + "\"";
        system(cmd.c_str());

        cout << "已发起任务进程：" << line << endl;
    }
    fin.close();
    cout << "所有并行任务已全部发起！" << endl << endl;
}

// 2. 等待校验 + 无序实时播报函数 CheckComplete
void CheckComplete()
{
    cout << "=====================================" << endl;
    cout << "正在等待所有多进程任务执行完成" << endl;
    cout << "=====================================" << endl;

    ifstream fin("CheckList.ini", ios::in);
    if(!fin.is_open())
    {
        cout << "错误：未找到 CheckList.ini 配置文件！" << endl;
        return;
    }

    string line;
    while(getline(fin, line))
    {
        if(line.empty()) continue;
        g_CheckFileList.push_back(line);
        g_FinishState.push_back(false);
    }
    fin.close();

    bool allDone = false;
    while(!allDone)
    {
        allDone = true;
        // 一轮遍历所有校验文件
        for(int i = 0; i < g_CheckFileList.size(); i++)
        {
            // 已经标记完成就跳过
            if(g_FinishState[i]) continue;

            const char* filePath = g_CheckFileList[i].c_str();
            if(IsFileExistA(filePath))
            {
                g_FinishState[i] = true;
                cout << g_CheckFileList[i] << " 已完成" << endl;
            }
            else
            {
                allDone = false;
            }
        }
        Sleep(150); // 短暂休眠，降低CPU占用
    }

    cout << endl << "全部任务校验通过！" << endl << endl;
}

// 3. 收尾清理 + 执行后置任务 AfterTask
void AfterTask()
{
    cout << "=====================================" << endl;
    cout << "所有任务完毕，即将执行收尾工作" << endl;
    cout << "=====================================" << endl;

    // 第一步：批量删除所有.check校验文件，重置状态
    cout << "正在清理任务校验标记文件..." << endl;
    for(int i = 0; i < g_CheckFileList.size(); i++)
    {
        DeleteSingleFileA(g_CheckFileList[i].c_str());
    }
    cout << "校验文件清理完成！" << endl << endl;

    // 第二步：读取并执行后置命令
    ifstream fin("After.ini", ios::in);
    if(!fin.is_open())
    {
        cout << "提示：未检测到 After.ini，跳过收尾指令" << endl;
        return;
    }

    string line;
    while(getline(fin, line))
    {
        if(line.empty()) continue;
        g_AfterCmdList.push_back(line);
        cout << "正在执行收尾指令：" << line << endl;
        system(line.c_str());
    }
    fin.close();
}

// 主调度函数
int main()
{
    // 关闭控制台快速闪烁，稳定ANSI输出
    SetConsoleTitleA("自研多任务构建器 - XJ380专用");

    BeginTask();     // 启动多进程
    CheckComplete(); // 无序等待校验
    AfterTask();     // 清理+收尾执行

    cout << endl << "=====================================" << endl;
    cout << "全部工作流程执行结束！" << endl;
    system("pause");
    return 0;
}