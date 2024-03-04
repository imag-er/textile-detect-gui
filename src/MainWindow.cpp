
#include "App.h"
#include "MainWindow.h"

#include "logger.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <commctrl.h>

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
	MoveWindow(hWnd, 100, 100, 1080, 720, TRUE);
	UpdateWindow(hWnd);
}

MainWindow::~MainWindow()
{
	Gdiplus::GdiplusShutdown(gdiplus_token);
	KillTimer(hWnd, uTimerId);
}

void MainWindow::onCommand(WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case CBN_SELCHANGE:
		HWND hComboBoxEx = (HWND)lParam;					 // 获取 ComboBoxEx 的句柄
		int selectedIndex = ComboBox_GetCurSel(hComboBoxEx); // 获取用户选择的项的索引
		LOG(selectedIndex);
		app->cm.cap.open(selectedIndex, cv::CAP_ANY); // 自动call release()
		break;
	}

	switch (LOWORD(wParam))
	{

	case MainWindow::TAKE_PHOTO:
	{
		cv::Mat m;
		bool res = app->cm.cap.read(m);
		if(res)
		{
			cv::imwrite("CameraCapture.jpg",m);
			MessageBoxW(0, (L"画面已保存至" + app->path + L"CameraCapture.jpg").c_str(), L"Data", MB_OK);
			
		}
		else
		{
			MessageBoxW(0, L"画面截取失败", L"Error", MB_ICONERROR);
		}
		break;
	}

	default:
		break;
	}
}

void MainWindow::onNotify(LPARAM lParam)
{
	auto pnmh = (NMHDR *)lParam;
	switch (pnmh->code)
	{
		break;
	}
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
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		window->onCommand(wParam, lParam);
		break;
	case WM_NOTIFY:
		window->onNotify(lParam);
		break;
	case WM_CREATE:
	{

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
	bool res = inst->cm.cap.read(m);

	if (res)
	{
		cv::resize(m, m, {512, 360}, 0, 0, cv::InterpolationFlags::INTER_LINEAR);
		cv::flip(m, m, 1);
		// 24 bits per pixel
		Gdiplus::Bitmap image(m.cols, m.rows, m.step, PixelFormat24bppRGB, m.data);
		graphics.DrawImage(&image, 10, 130);
	}
	return;
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

	hTakePhoto = CreateWindowW(
		WC_BUTTONW,
		L"拍照",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 10, 100, 100,
		hWnd,
		(HMENU)MainWindow::TAKE_PHOTO,
		app->hInstance,
		nullptr);

	hComboBox = CreateWindowExW(
		0,
		WC_COMBOBOXEXW,
		L"111",
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_TABSTOP,
		120, 10, 100, 100,
		hWnd,
		(HMENU)MainWindow::CAP_SELECT,
		app->hInstance,
		nullptr);

	constexpr auto cap_fps = 20; // 帧率
	constexpr auto elapse = 1000 / cap_fps;
	uTimerId = SetTimer(
		hWnd,
		MainWindow::TIMER_CAP,
		elapse,
		[](HWND, UINT, UINT_PTR, DWORD)
		{
			static auto window = App::Instance()->mainwindow;
			window->onPaint(GetDC(window->hWnd));
			return;
		});

	COMBOBOXEXITEM item;
	ZeroMemory(&item, sizeof(item));
	item.mask = CBEIF_TEXT;
	auto cap_list = app->cm.get_camera_list();

	for (auto it = cap_list.rbegin(); it != cap_list.rend(); it++)
	{
		auto lng = it->size() + 1;
		item.pszText = new wchar_t[lng];
		StringCchCopyW(item.pszText, lng, it->c_str());
		SendMessage(hComboBox, CBEM_INSERTITEM, 0, (LPARAM)&item); // 插入选项
		delete[] item.pszText;
	}
}
