#include <EasyX_Draw.h>
#include <cstdio>
#include <ctime>

constexpr int maxn = 1000;

char buffer[maxn];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	std::pair<int, int> window_size = createWin(-1, -1, -1, -1, true);

	FILE* video_tape;
	fopen_s(&video_tape, "Visual.txt", "r");
	if (video_tape == nullptr) {
		puts("File error");
		Sleep(100000);
		return 0;
	}

	BeginBatchDraw();
	cleardevice();
	setCurrentColor(Color(0, 200, 200));
	FlushBatchDraw();

	double lst_time = (double)clock() / CLOCKS_PER_SEC, time_step;
	fgets(buffer, maxn, video_tape);
	sscanf_s(buffer, "%lf", &time_step);
	while (fgets(buffer, maxn, video_tape)) {
		if (strstr(buffer, "Frame:") != nullptr) {
			FlushBatchDraw();
			while ((double)clock() / CLOCKS_PER_SEC - lst_time < time_step);
			lst_time = (double)clock() / CLOCKS_PER_SEC;
			cleardevice();
		}
		else {
			if (strstr(buffer, "circle") != nullptr) {
				double x, y, r;
				sscanf_s(buffer, "circle %lf %lf %lf", &x, &y, &r);
				drawSolidCircle(Circle(Vector2D(x, y), r), Color(200, 0, 200));
			}
			if (strstr(buffer, "dot") != nullptr) {
				double x, y, r = 0;
				int c_space = 0, l = std::strlen(buffer);
				for (int i = 0; i < l; ++i) c_space += buffer[i] == ' ';
				if (c_space > 2) sscanf_s(buffer, "dot %lf %lf %lf", &x, &y, &r);
				else sscanf_s(buffer, "dot %lf %lf", &x, &y);
				drawDot(Vector2D(x, y), max(1.5, r));
			}
			if (strstr(buffer, "seg") != nullptr) {
				double x1, y1, x2, y2;
				sscanf_s(buffer, "seg %lf %lf %lf %lf", &x1, &y1, &x2, &y2);
				drawLine(Segment(Vector2D(x1, y1), Vector2D(x2, y2)), 1.5);
			}
		}
	}
	FlushBatchDraw();
	while (!(GetAsyncKeyState(VK_F12) & 0x8000));
	EndBatchDraw();
	closegraph();

	fclose(video_tape);

	return 0;
}