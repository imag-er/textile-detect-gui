
#include "App.h"
#include "MainWindow.h"

#include "logger.h"

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);

MainWindow::MainWindow(const App *app_, const std::string &name_)
	: name(name_),
	  hWnd(0),
	  app(app_)
{

	checkGdiplus();
	// checkVideoCapture();
	checkApp();
	checkWindow();

	ShowWindow(hWnd, SW_SHOWNORMAL);
	MoveWindow(hWnd, 100, 100, 800, 600, TRUE);
	UpdateWindow(hWnd);
}

LRESULT CALLBACK MainWindowProc(
	HWND hWnd,
	UINT msgCode,
	WPARAM wParam,
	LPARAM lParam)
{
	static auto inst = App::Instance();
	static auto window = inst->mainwindow;

	switch (msgCode)
	{
	case WM_DESTROY:
		Gdiplus::GdiplusShutdown(window->gdiplus_token);
		PostQuitMessage(0);
		break;
	case WM_CREATE:
	{
		// 注意这里CreateWindow还没返回，hwnd没有值

		// 绘制计时器
		constexpr auto cap_fps = 20;
		constexpr auto eclapse = 1000 / cap_fps;
		SetTimer(
			hWnd,
			MainWindow::WNDID::TIMER_CAP,
			eclapse,
			[](HWND, UINT, UINT_PTR, DWORD) {
				static auto window = App::Instance()->mainwindow;
				window->onPaint(GetDC(window->hWnd));
				return;
			});
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		window->onPaint(hdc);
		EndPaint(hWnd, &ps);
		break;
	}

	default:
		return DefWindowProcW(hWnd, msgCode, wParam, lParam);
	}
	return DefWindowProcW(hWnd, msgCode, wParam, lParam);
}

void MainWindow::onPaint(HDC hdc)
{
	static auto inst = App::Instance();
	Gdiplus::Graphics graphics(hdc);
	cv::Mat m;
	bool res = inst->cap->read(m);

	// 24 bits per pixel
	Gdiplus::Bitmap image(m.cols, m.rows, m.step, PixelFormat24bppRGB, m.data);
	graphics.DrawImage(&image, 0, 0);
}

bool MainWindow::checkGdiplus()

{
	Gdiplus::Status st = Gdiplus::GdiplusStartup(&gdiplus_token, &gdiplus_startup_input, NULL);
	if (st != Gdiplus::Status::Ok)
	{
		LOG("cannot start gdi+ err code:", st);

		return false;
	}
	LOG("successfully started gdi+");
	return true;
}

bool MainWindow::checkWindow()
{
	createWin32Window();
	auto res = GetLastError();
	if (hWnd == 0)
	{
		LOG("create window failed\terr code:", res);
		return false;
	}

	LOG("successfully created window ");
	return true;
}
bool MainWindow::checkApp()
{
	if (app == nullptr)
	{
		LOG("no App instance");
		return false;
	}
	LOG("got app instance");
	return true;
}
void MainWindow::createWin32Window()
{
	WNDCLASSEXW w{0};
	w.cbSize = sizeof(WNDCLASSEXW);

	w.hInstance = app->hInstance;

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
						   NULL, NULL, app->hInstance, NULL);
}
