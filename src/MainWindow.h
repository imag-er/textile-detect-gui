#pragma once
#ifndef MainWindowH
#define MainWindowH 1

#include "App.h"

#include <windows.h>
#include <string>

#include <vector>
#include <objidl.h>

#include <gdiplus.h>
#pragma comment(lib,"Gdiplus.lib")

#include <tuple>
class MainWindow
{
public:
	MainWindow(const std::string& );
	operator bool() {return hWnd == 0;}
	HWND hWnd;
	HWND hCapWnd;
	const std::string& name;

	struct Device
	{
		std::wstring device_name;
		std::wstring device_version;
	};

	ULONG_PTR gdiplus_token;
	Gdiplus::GdiplusStartupInput gdiplus_startup_input;

	void startStreaming();



};
#endif