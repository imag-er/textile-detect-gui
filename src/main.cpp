#include <windows.h>
#include "App.h"
#include "CameraManager.h"
#include "logger.h"
INT WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	LPWSTR lpwCmdLine,
	INT nCmdShow
)
{
	auto inst = App::Instance(hInstance);

	// CameraManager cm;
	// auto res = cm.get_camera_list();

	// for (auto k : res)
	// {
	// 	LOG(k);
	// }
	// cm.get_camera_list();
	inst->makeComponents();
	return inst->msgLoop();
}