// SetWindowsHook.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
/*
Windows操作系统向用户提供GUI，它以事件驱动方式工作，在操作系统中借助键盘，鼠标，选择菜单，按钮，以及移动鼠标，
改变窗口大小与位置等都是事件，发生这样的事件时，OS会把事先定义好的消息发送给相应的应用程序，应用程序分析收到
的信息后执行相应动作，也就是说，敲击键盘时，消息会从OS移动到应用程序，所谓的“消息钩子”就在此间偷看这些消息
为了帮助各位进一步理解它，下面以键盘消息为例说明

SetWindowsHookEx(
    int idHook,
    HOOKPROC lpfn,
    HINSTANCE hMod.
    DWORD dwThreadId
)

*/
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#define DEF_DLL_NAME "KeyHook.dll"
#define DEF_HOOKSTART "HookStart"
#define DEF_HOOKSTOP  "HookStop"

typedef void(*PFN_HOOKSTART)();
typedef void(*PFN_HOOKSTOP)();

int main()
{
    HMODULE hDll = NULL;
    PFN_HOOKSTART HookStart = NULL;
    PFN_HOOKSTOP HookStop = NULL;

    //加载key HooK.DLL
    hDll = LoadLibraryA(DEF_DLL_NAME);
    if (hDll)
    {
        //获取导出函数地址
        HookStart = (PFN_HOOKSTART)GetProcAddress(hDll, DEF_HOOKSTART);
        HookStop = (PFN_HOOKSTOP)GetProcAddress(hDll, DEF_HOOKSTOP);

        //开始勾取
        HookStart();
        //等待直到用户输入“q”
        printf("Press 'q' to quit! \n");
        while (_getch() != 'q')
        {

        }

        //停止勾取
        HookStop();

        //卸载KetHook.DLL
        FreeLibrary(hDll);
    }
   

}

