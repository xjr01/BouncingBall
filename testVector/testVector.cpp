#include "pch.h"
#include "CppUnitTest.h"
#include <vector2d.h>
#include <EasyX_Draw.h>
#include <Windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testVector
{
	TEST_CLASS(testVector)
	{
	public:
		
		TEST_METHOD(TestAdd)
		{
			Vector2D a(2, 3), b(4, 5);
			Assert::IsTrue(a + b == Vector2D(6, 8));
		}
	};
	TEST_CLASS(testDraw) {
	public:

		TEST_METHOD(TestDraw) {
			createWin();
			BeginBatchDraw();
			setCurrentColor(Color(0, 0, 127));
			drawLine(Segment(Vector2D(100, 80), Vector2D(100, 280)), 10);
			drawCircle(Vector2D(100, 500), 120, 8);
			drawCircle(Vector2D(1300, 500), 120, 1.5);
			drawSolidCircle(Vector2D(500, 400), 120);
			drawDot(Vector2D(900, 180), 9);
			cleardevice();
			//drawPolygon(std::vector<Vector2D>{ { 134, 154 }, { 725, 166 }, { 263, 532 }, { 484, 33 }, { 531, 568 } }, 1);
			drawSolidPolygon(std::vector<Vector2D>{ { 134, 154 }, { 725, 166 }, { 263, 532 }, { 484, 33 }, { 531, 568 } });
			FlushBatchDraw();
			EndBatchDraw();
			while (!GetAsyncKeyState(VK_ESCAPE));
			closegraph();
			exit(0);
		}
	};
}
