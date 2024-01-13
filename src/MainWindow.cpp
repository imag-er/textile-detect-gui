
#include "App.h"
#include "MainWindow.h"

#include "logger.h"

#include <vfw.h>

#pragma comment(lib, "Vfw32.lib")
enum CHILDWINDOWID
{
	CAP = 0,
};
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);

MainWindow::MainWindow(const std::string &name_) : name(name_), hWnd(0)
{
	auto inst = App::Instance();

	Gdiplus::Status st = Gdiplus::GdiplusStartup(&gdiplus_token,&gdiplus_startup_input,NULL);
	if (st != Gdiplus::Status::Ok)
	{
		LOG("cannot start gdi+ err code:",st);
		return;
	}
	WNDCLASSEXW w{0};
	w.cbSize = sizeof(WNDCLASSEXW);

	w.hInstance = App::Instance()->hInstance;

	w.lpfnWndProc = (WNDPROC)MainWindowProc; // TODO

	w.lpszClassName = L"MainWindow";
	
	w.style = CS_HREDRAW | CS_VREDRAW;


	w.hIcon = LoadIcon(NULL, IDI_APPLICATION);
 	w.hCursor = LoadCursor(NULL, IDC_ARROW);
 	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);


	auto res = RegisterClassExW(&w);
	if (res == 0)
	{

		LOG("registration failed\terr code:", res);
		return;
	}
	LOG("successfully registered");

	hWnd = CreateWindowExW(0, w.lpszClassName, strconv(name).c_str(), WS_OVERLAPPEDWINDOW,
						   CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
						   NULL, NULL, inst->hInstance, NULL);
	if (hWnd == 0)
	{
		LOG("create window failed\terr code:", res);
		return;
	}

	ShowWindow(hWnd, SW_SHOWNORMAL);
	MoveWindow(hWnd, 100, 100, 800, 600, TRUE);
	UpdateWindow(hWnd);
}

void MainWindow::startStreaming()
{
	static auto inst = App::Instance();
	HWND hCapWnd = inst->mainwindow->hCapWnd;



	return;
}

LRESULT CALLBACK MainWindowProc(
	HWND hWnd,
	UINT msgCode,
	WPARAM wParam,
	LPARAM lParam)
{
	static auto inst = App::Instance();
	switch (msgCode)
	{
	case WM_DESTROY:
		Gdiplus::GdiplusShutdown(inst->mainwindow->gdiplus_token);
		PostQuitMessage(0);
		break;
	case WM_CREATE:
	{
		// LOG(L"running path",inst->app_path);
		// 注意这里CreateWindow还没返回，hwnd没有值
		inst->mainwindow->hCapWnd = capCreateCaptureWindowW(L"capWindow",
															WS_CHILD | WS_VISIBLE,
															0, 0, 240, 160,
															hWnd,
															CAP);
		if (inst->mainwindow->hCapWnd == 0)
		{
			LOG(L"create cap window failed\terr code:", GetLastError());
			return 0;
		}
		// inst->mainwindow->startStreaming();

		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd,&ps);
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::Image image(L"./11.bmp");

		Gdiplus::Status st = image.GetLastStatus();
		if (st != Gdiplus::Ok)
		{	
			LOG(L"cannot open 11.bmp err code:",st);
			
			return 1;
		}
		graphics.DrawImage(&image,0,0);
		EndPaint(hWnd,&ps);
		return 0;
	}

	default:
		return DefWindowProcW(hWnd, msgCode, wParam, lParam);
	}
	return DefWindowProcW(hWnd, msgCode, wParam, lParam);
}