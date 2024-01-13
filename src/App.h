#pragma	once
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

	auto msgLoop()-> decltype(MSG::wParam);
	static HINSTANCE hInstance;
	MainWindow mainwindow;

private:
	class Logger{
	public:
		using pstreambuf = std::streambuf*;
		pstreambuf old_cout_buf,old_cerr_buf,old_cin_buf;
		std::ofstream outfs,errfs;
		std::ifstream infs;
		Logger();
		~Logger();
	} logger;
	
	App(HINSTANCE) ;
	static std::shared_ptr<App> instance;
};