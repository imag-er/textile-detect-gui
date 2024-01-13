#pragma	once
#ifndef APPH
#define APPH 1

#define WIN32_LEAN_AND_MEAN
#define _UNICODE
// 尽量少使用 还是看情况分配
constexpr auto STRINGLENGTH = 512;



#include <windows.h>
#include <memory>
#include <iostream>
#include <fstream>
#include "MainWindow.h"


class App
{
public:
	static std::shared_ptr<App> Instance(HINSTANCE hInstance = 0) ;
	~App();

	WPARAM msgLoop();
	VOID makeComponents();

	static HINSTANCE hInstance;
	// static std::wstring app_path;
	static std::wstring path;

	MainWindow* mainwindow;

private:
	class Logger{
	public:
		using pstreambuf = std::wstreambuf*;
		pstreambuf old_cout_buf,old_cerr_buf,old_cin_buf;
		std::wofstream outfs,errfs;
		std::wifstream infs;
		Logger();
		~Logger();
	} logger;


	App(HINSTANCE) ;
	static std::shared_ptr<App> instance;
};
#endif