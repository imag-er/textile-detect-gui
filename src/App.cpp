#include "App.h"
#include "logger.h"
std::shared_ptr<App> App::instance = nullptr;
HINSTANCE App::hInstance = NULL;

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
	logger(),mainwindow()
{

// 应用初始化
	this->hInstance = hInstance;

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
	// errfs.open("log/error.log", std::ios::app);
// 重定向输入输出
	old_cout_buf = std::cout.rdbuf(outfs.rdbuf());
	old_cin_buf = std::cin.rdbuf(infs.rdbuf());
	// old_cerr_buf = std::cerr.rdbuf(errfs.rdbuf());
}
App::~App()
{

}
App::Logger::~Logger()
{
	// 关闭文件流
	outfs.close();
	infs.close();
	// errfs.close();
	
	// 恢复旧的ostream buf
	std::cout.rdbuf(old_cout_buf);
	std::cin.rdbuf(old_cin_buf);
	// std::cerr.rdbuf(old_cerr_buf);
}
auto App::msgLoop() -> decltype(MSG::wParam)
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
