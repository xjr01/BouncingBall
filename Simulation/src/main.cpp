#include "physics.h"
#include <EasyX_Draw.h>
#include <ctime>
#include <math_ex.h>
#include <numeric>

const double draw_time_step = .02;

Ball the_ball;
std::vector<WallDot> dots;
std::vector<WallSegment> segs;

void draw() {
	cleardevice();
	drawSolidCircle(the_ball.shape, Color(200, 0, 200));
	for (const auto& dot : dots) drawDot(dot.p, 1.5);
	for (const auto& seg : segs) drawLine(seg.seg, 1.5);
	FlushBatchDraw();
	return;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	std::pair<int, int> window_size = createWin(-1, -1, -1, -1, true);

	BeginBatchDraw();
	cleardevice();
	setCurrentColor(Color(0, 200, 200));
	FlushBatchDraw();

	the_ball.set_m(1);
	the_ball.set_shape(Circle(Vector2D(367, 30), 10));
	the_ball.set_v(Vector2D(0, 0));
	for (int y = 660; y <= 850; y += 60)
		for (int x = 30; x <= 1200; x += 30)
			dots.push_back(WallDot(Vector2D(x, y)));
	for (int y = 690; y <= 850; y += 60)
		for (int x = 45; x <= 1200; x += 30)
			dots.push_back(WallDot(Vector2D(x, y)));
	segs.push_back(WallSegment(Segment(Vector2D(30, 850), Vector2D(1400, 850))));

	segs.push_back(WallSegment(Segment(Vector2D(300, 200), Vector2D(250, 300))));
	segs.push_back(WallSegment(Segment(Vector2D(300, 400), Vector2D(250, 300))));
	segs.push_back(WallSegment(Segment(Vector2D(300, 400), Vector2D(350, 300))));
	segs.push_back(WallSegment(Segment(Vector2D(300, 200), Vector2D(350, 300))));

	segs.push_back(WallSegment(Segment(Vector2D(200, 350), Vector2D(150, 450))));
	segs.push_back(WallSegment(Segment(Vector2D(200, 550), Vector2D(150, 450))));
	segs.push_back(WallSegment(Segment(Vector2D(200, 550), Vector2D(250, 450))));
	segs.push_back(WallSegment(Segment(Vector2D(200, 350), Vector2D(250, 450))));

	segs.push_back(WallSegment(Segment(Vector2D(400, 350), Vector2D(350, 450))));
	segs.push_back(WallSegment(Segment(Vector2D(400, 550), Vector2D(350, 450))));
	segs.push_back(WallSegment(Segment(Vector2D(400, 550), Vector2D(450, 450))));
	segs.push_back(WallSegment(Segment(Vector2D(400, 350), Vector2D(450, 450))));

	Vector2D g(0, 98);
	draw();

	double real_lst_time = (double)clock() / CLOCKS_PER_SEC, real_cur_time, // draw time
		time_passed = 0, // simulation time
		speed_rate = 1.0; // draw speed
	bool drew = false;
	while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000)) {
		if (time_passed < draw_time_step) {
			double time_step_remaining = time_step;
			while (DOUBLE_EPS::gt(time_step_remaining, 0)) {
				if (GetAsyncKeyState(VK_UP) & 0x8000) the_ball.addForce(Vector2D(0, -500) * the_ball.get_mass());
				if (GetAsyncKeyState(VK_DOWN) & 0x8000) the_ball.addForce(Vector2D(0, 120) * the_ball.get_mass());
				if (GetAsyncKeyState(VK_LEFT) & 0x8000) the_ball.addForce(Vector2D(-120, 0) * the_ball.get_mass());
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000) the_ball.addForce(Vector2D(120, 0) * the_ball.get_mass());
				the_ball.addForce(g * the_ball.get_mass());
				std::pair<double, std::shared_ptr<Wall>> first_collide =
					std::make_pair(std::numeric_limits<double>::infinity(), std::shared_ptr<Wall>()),
					the_collide;
				for (const auto& dot : dots) {
					the_collide = collisionDetect(the_ball, dot);
					if (the_collide.first < first_collide.first)
						first_collide = the_collide;
				}
				for (const auto& seg : segs) {
					the_collide = collisionDetect(the_ball, seg);
					if (the_collide.first < first_collide.first)
						first_collide = the_collide;
				}
				double time_forward = first_collide.first;
				if (time_forward <= time_step_remaining) {
					the_ball.integrate(time_forward);
					first_collide.second->collisionRespond(the_ball);
					time_step_remaining -= time_forward;
				}
				else {
					the_ball.integrate(time_step_remaining);
					time_step_remaining = 0;
				}
			}
			time_passed += time_step;
		}
		if (!((GetAsyncKeyState(VK_F3) & 0x8001) ^ 0x8001)) speed_rate *= 2;
		if (!((GetAsyncKeyState(VK_F4) & 0x8001) ^ 0x8001)) speed_rate /= 2;
		real_cur_time = (double)clock() / CLOCKS_PER_SEC;
		if (real_cur_time - real_lst_time >= draw_time_step / speed_rate && time_passed >= draw_time_step) {
			real_lst_time = real_cur_time;
			time_passed = 0;
			draw();
		}
	}
	EndBatchDraw();
	closegraph();

	return 0;
}