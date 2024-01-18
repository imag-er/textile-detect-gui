#pragma once
#ifndef MainWindowH
#define MainWindowH 1

#include "App.h"

#include <windows.h>
#include <string>

#include <vector>
#include <objidl.h>

#include <gdiplus.h>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#pragma comment(lib,"Gdiplus.lib")

#include <tuple>

class App;
class MainWindow
{
public:
	
	MainWindow(const App*,const std::string&);

	const App* app ;

	// win32 window
	HWND hWnd;
	const std::string& name;
	operator bool() {return static_cast<bool>(hWnd);}
	void createWin32Window();

	void onPaint(HDC);

	// gdi+
	ULONG_PTR gdiplus_token;
	Gdiplus::GdiplusStartupInput gdiplus_startup_input;


	// opencv 
	

	// check and ensure
	bool checkGdiplus();
	
	bool checkWindow();
	bool checkApp();
	
	enum WNDID{
		TIMER_CAP
	};
};
#endif