#include "physics.h"
#include "BouncingBallDriver.h"
#include <EasyX_Draw.h>

BouncingBallDriver driver;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	driver.main();

	return 0;
}