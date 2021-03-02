#include "physics.h"
#include <EasyX_Draw.h>
#include <ctime>
#include <math_ex.h>
#include <numeric>

const double draw_time_step = .02;

Ball the_ball;
std::vector<WallDot> dots;

void draw() {
	cleardevice();
	drawSolidCircle(the_ball.shape, Color(200, 0, 200));
	for (const auto& dot : dots) drawDot(dot.p, 1.5);
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
	the_ball.set_shape(Circle(Vector2D(267, 30), 10));
	the_ball.set_v(Vector2D(0, 0));
	for (int y = 200; y <= 850; y += 60)
		for (int x = 30; x <= 1200; x += 30)
			dots.push_back(WallDot(Vector2D(x, y)));
	for (int y = 230; y <= 850; y += 60)
		for (int x = 45; x <= 1200; x += 30)
			dots.push_back(WallDot(Vector2D(x, y)));
	for (int x = 30; x <= 1200; x += 10)
		dots.push_back(WallDot(Vector2D(x, 850)));
	Vector2D g(0, .3);
	draw();

	double real_lst_time = (double)clock() / CLOCKS_PER_SEC, real_cur_time, // draw time
		time_passed = 0; // simulation time
	bool drew = false;
	while (!GetAsyncKeyState(VK_ESCAPE)) {
		if (time_passed < draw_time_step) {
			double time_step_remaining = time_step;
			while (DOUBLE_EPS::gt(time_step_remaining, 0)) {
				the_ball.addForce(g * the_ball.get_mass());
				std::pair<double, Wall*> first_collide =
					std::make_pair(std::numeric_limits<double>::infinity(), (Wall*)nullptr),
					the_collide;
				for (const auto& dot : dots) {
					the_collide = collisionDetect(the_ball, dot);
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
		real_cur_time = (double)clock() / CLOCKS_PER_SEC;
		if (real_cur_time - real_lst_time >= draw_time_step && time_passed >= draw_time_step) {
			real_cur_time = real_lst_time;
			time_passed = 0;
			draw();
		}
	}
	EndBatchDraw();
	closegraph();

	return 0;
}