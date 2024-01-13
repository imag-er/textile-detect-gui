#include "App.h"
#include "logger.h"
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")


std::shared_ptr<App> App::instance = nullptr;
HINSTANCE App::hInstance = NULL;
std::wstring App::path{};

std::shared_ptr<App> App::Instance(HINSTANCE hInstance)
{
	if (!instance)
	{
		// 这里如果用工厂模式会调用constructor 会寄
		instance = std::shared_ptr<App>(new App(hInstance));
	}


	return instance;
}

App::App(HINSTANCE hInstance) :
	logger()
{
// 在这里不能初始化app的component 否则会递归构造

// 应用初始化
	this->hInstance = hInstance;

// 获取exe文件地址
	WCHAR buffer[512];
	GetModuleFileNameW(0,buffer,512);	
	this->path = buffer;
	LOG(buffer);

	LOG("Instance Constructed HANDLE:", hInstance);
	// logfs = init_log();

}
App::Logger::Logger()
{
	

// logger初始化
	AllocConsole();

// 打开文件流
	outfs.open("conout$", std::ios::app);
	infs.open("conin$", std::ios::in);
	errfs.open("log/error.log", std::ios::app);
// 重定向输入输出
	old_cout_buf = std::wcout.rdbuf(outfs.rdbuf());
	old_cin_buf = std::wcin.rdbuf(infs.rdbuf());
	old_cerr_buf = std::wcerr.rdbuf(errfs.rdbuf());
}
App::~App()
{

}
App::Logger::~Logger()
{
	// 关闭文件流
	outfs.close();
	infs.close();
	errfs.close();
	
	// 恢复旧的ostream buf
	std::wcout.rdbuf(old_cout_buf);
	std::wcin.rdbuf(old_cin_buf);
	std::wcerr.rdbuf(old_cerr_buf);
}
WPARAM App::msgLoop() 
{
// 主消息循环
	MSG msg;
	while (GetMessageW(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return msg.wParam;
}

VOID App::makeComponents()
{
	mainwindow = new MainWindow("纺织品检测");
}