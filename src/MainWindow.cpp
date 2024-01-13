#include "App.h"
#include "MainWindow.h"

#include "logger.h"
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);


MainWindow::MainWindow()
{

	WNDCLASSEXW w{0};
	w.cbSize = sizeof(WNDCLASSEXW);

	w.hInstance = App::Instance()->hInstance;

	w.lpfnWndProc = (WNDPROC)MainWindowProc; // TODO

	w.lpszClassName = L"MainWindow";

	auto res = RegisterClassExW(&w);
	if (res != 0)
	{
		LOG("registration failed, err code:",res);
	}
	LOG("successfully registered");

	while (1) {}
}

LRESULT CALLBACK MainWindowProc(
	HWND hWnd,
	UINT msgCode,
	WPARAM wParam,
	LPARAM lParam)
{
	
	switch (msgCode)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	default:
		return DefWindowProcW(hWnd, msgCode, wParam, lParam);
	}
	return DefWindowProcW(hWnd, msgCode, wParam, lParam);
}