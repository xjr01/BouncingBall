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

void rotate(double angle, Vector2D center) {
	the_ball.shape.p.rotate_with_center_(angle, center);
	for (auto& dot : dots) dot.p.rotate_with_center_(angle, center);
	for (auto& seg : segs)
		seg.seg = Segment(
			seg.seg.p1.rotate_with_center(angle, center),
			seg.seg.p2.rotate_with_center(angle, center)
		);
	return;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	std::pair<int, int> window_size = createWin(-1, -1, -1, -1, true);

	BeginBatchDraw();
	cleardevice();
	setCurrentColor(Color(0, 200, 200));
	FlushBatchDraw();

	printf("%d %d\n", window_size.first, window_size.second);
	the_ball.m = 1;
	the_ball.shape = Circle(Vector2D(960, 540) / 1920 * 1536, 10);
	the_ball.v = Vector2D(0, 0);
	for (int y = 225; y <= 860; y += 60)
		for (int x = 645; x <= 1280; x += 30)
			dots.push_back(WallDot(Vector2D(x, y) / 1920 * 1536));
	for (int y = 255; y <= 860; y += 60)
		for (int x = 660; x <= 1280; x += 30)
			dots.push_back(WallDot(Vector2D(x, y) / 1920 * 1536)); // */
	segs.push_back(WallSegment(Segment(Vector2D(580, 160) / 1920 * 1536, Vector2D(1340, 160) / 1920 * 1536)));
	segs.push_back(WallSegment(Segment(Vector2D(1340, 920) / 1920 * 1536, Vector2D(1340, 160) / 1920 * 1536)));
	segs.push_back(WallSegment(Segment(Vector2D(1340, 920) / 1920 * 1536, Vector2D(580, 920) / 1920 * 1536)));
	segs.push_back(WallSegment(Segment(Vector2D(580, 160) / 1920 * 1536, Vector2D(580, 920) / 1920 * 1536)));

	/*segs.push_back(WallSegment(Segment(Vector2D(300, 200), Vector2D(250, 300))));
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
	segs.push_back(WallSegment(Segment(Vector2D(400, 350), Vector2D(450, 450))));*/

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
				if (GetAsyncKeyState(VK_UP) & 0x8000) the_ball.addForce(Vector2D(0, -500) * the_ball.m);
				if (GetAsyncKeyState(VK_DOWN) & 0x8000) the_ball.addForce(Vector2D(0, 120) * the_ball.m);
				if (GetAsyncKeyState(VK_LEFT) & 0x8000) the_ball.addForce(Vector2D(-120, 0) * the_ball.m);
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000) the_ball.addForce(Vector2D(120, 0) * the_ball.m);
				the_ball.addForce(g * the_ball.m);
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
		double fast_rotate = .001, slow_rotate = .0001,
			rotate_rate = (GetAsyncKeyState(VK_LCONTROL) & 0x8000) ? fast_rotate : slow_rotate;
		if (GetAsyncKeyState('A') & 0x8000)
			rotate(-rotate_rate / 180 * pi, Vector2D(window_size.first / 2, window_size.second / 2));
		if (GetAsyncKeyState('D') & 0x8000)
			rotate(rotate_rate / 180 * pi, Vector2D(window_size.first / 2, window_size.second / 2));
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