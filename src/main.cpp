#include <windows.h>
#include "App.h"

INT WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	LPWSTR lpwCmdLine,
	INT nCmdShow
)
{
	App::Instance(hInstance);

	return App::Instance()->msgLoop();
}