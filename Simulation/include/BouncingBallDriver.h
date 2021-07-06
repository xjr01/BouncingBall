#include "physics.h"
#include <EasyX_Draw.h>
#include <ctime>
#include <math_ex.h>
#include <numeric>
#include <algorithm>

#define pWall std::shared_ptr<Wall>

class BouncingBallDriver {
private:
	static constexpr double simulation_time_step = .01;
	static constexpr double draw_time_step = .02;

	std::vector<Ball> balls;
	std::vector<WallDot> dots;
	std::vector<WallSegment> segs;
	Vector2D g;

	// the id of the ball, and the wall object it run into
	std::vector<std::pair<int, pWall>> wall_collide;
	double wall_collide_time;
	// the id of the two balls that collide
	std::vector<std::pair<int, int>> ball_collide;
	double ball_collide_time;

	void draw() {
		cleardevice();
		for (const auto& the_ball : balls)
			drawSolidCircle(the_ball.shape, Color(200, 0, 200));
		for (const auto& dot : dots) drawDot(dot.p, 1.5);
		for (const auto& seg : segs) drawLine(seg.seg, 1.5);
		FlushBatchDraw();
		return;
	}

	void rotate(double angle, Vector2D center) {
		for (auto& the_ball : balls)
			the_ball.shape.p.rotate_with_center_(angle, center);
		for (auto& dot : dots) dot.p.rotate_with_center_(angle, center);
		for (auto& seg : segs)
			seg.seg = Segment(
				seg.seg.p1.rotate_with_center(angle, center),
				seg.seg.p2.rotate_with_center(angle, center)
			);
		return;
	}

	void initialize() {
		balls.clear();
		dots.clear();
		segs.clear();

		balls.push_back(
			Ball(
				Circle(Vector2D(808, 200) / 1920 * 1536, 10),	// shape
				Vector2D(0, 0),									// velocity
				1.0												// mass
			));
		balls.push_back(
			Ball(
				Circle(Vector2D(850, 200) / 1920 * 1536, 8),
				Vector2D(0, 0),
				2.0
			));
		balls.push_back(
			Ball(
				Circle(Vector2D(900, 200) / 1920 * 1536, 6),
				Vector2D(0, 0),
				1.5
			));

			// cube wall 1
		segs.push_back(WallSegment(Segment(Vector2D(800, 400) / 1920 * 1536, Vector2D(900, 500) / 1920 * 1536), .6));
		segs.push_back(WallSegment(Segment(Vector2D(900, 500) / 1920 * 1536, Vector2D(800, 600) / 1920 * 1536), .6));
		segs.push_back(WallSegment(Segment(Vector2D(800, 600) / 1920 * 1536, Vector2D(700, 500) / 1920 * 1536), .6));
		segs.push_back(WallSegment(Segment(Vector2D(700, 500) / 1920 * 1536, Vector2D(800, 400) / 1920 * 1536), .6));
		// cube wall 2
		segs.push_back(WallSegment(Segment(Vector2D(1120, 400) / 1920 * 1536, Vector2D(1220, 500) / 1920 * 1536), .6));
		segs.push_back(WallSegment(Segment(Vector2D(1220, 500) / 1920 * 1536, Vector2D(1120, 600) / 1920 * 1536), .6));
		segs.push_back(WallSegment(Segment(Vector2D(1120, 600) / 1920 * 1536, Vector2D(1020, 500) / 1920 * 1536), .6));
		segs.push_back(WallSegment(Segment(Vector2D(1020, 500) / 1920 * 1536, Vector2D(1120, 400) / 1920 * 1536), .6));
		for (int y = 675; y <= 860; y += 60)
			for (int x = 645; x <= 1280; x += 30)
				dots.push_back(WallDot(Vector2D(x, y) / 1920 * 1536, .2));
		for (int y = 705; y <= 860; y += 60)
			for (int x = 660; x <= 1280; x += 30)
				dots.push_back(WallDot(Vector2D(x, y) / 1920 * 1536, .2)); // */
		segs.push_back(WallSegment(Segment(Vector2D(580, 160) / 1920 * 1536, Vector2D(1340, 160) / 1920 * 1536), .2));
		segs.push_back(WallSegment(Segment(Vector2D(1340, 920) / 1920 * 1536, Vector2D(1340, 160) / 1920 * 1536), .2));
		segs.push_back(WallSegment(Segment(Vector2D(1340, 920) / 1920 * 1536, Vector2D(580, 920) / 1920 * 1536), .2));
		segs.push_back(WallSegment(Segment(Vector2D(580, 160) / 1920 * 1536, Vector2D(580, 920) / 1920 * 1536), .2));

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

		g = Vector2D(0, 98);
		return;
	}

	void wall_collision_detect() {
		std::pair<double, pWall> the_collide, first_collide;
		for (int i = 0; i < balls.size(); ++i) {
			const Ball& the_ball = balls[i];
			first_collide = std::make_pair(std::numeric_limits<double>::infinity(), std::shared_ptr<Wall>());
			for (const auto& dot : dots) {
				the_collide = collisionDetect(the_ball, dot);
				if (the_collide.first < first_collide.first)
					first_collide = the_collide;
			}
			if (DOUBLE_EPS::lt(first_collide.first, wall_collide_time)) {
				wall_collide_time = first_collide.first;
				wall_collide.clear();
				wall_collide.push_back(make_pair(i, first_collide.second));
			}
			else if (DOUBLE_EPS::eq(first_collide.first, wall_collide_time))
				wall_collide.push_back(make_pair(i, first_collide.second));
		}
		for (int i = 0; i < balls.size(); ++i) {
			const Ball& the_ball = balls[i];
			first_collide = std::make_pair(std::numeric_limits<double>::infinity(), std::shared_ptr<Wall>());
			for (const auto& seg : segs) {
				the_collide = collisionDetect(the_ball, seg);
				if (the_collide.first < first_collide.first)
					first_collide = the_collide;
			}
			if (DOUBLE_EPS::lt(first_collide.first, wall_collide_time)) {
				wall_collide_time = first_collide.first;
				wall_collide.clear();
				wall_collide.push_back(make_pair(i, first_collide.second));
			}
			else if (DOUBLE_EPS::eq(first_collide.first, wall_collide_time))
				wall_collide.push_back(make_pair(i, first_collide.second));
		}
		return;
	}

	void ball_collision_detect() {
		for (int i = 0; i < balls.size(); ++i)
			for (int j = i + 1; j < balls.size(); ++j) {
				Ball tmp_ball(balls[i]);
				tmp_ball.shape.r += balls[j].shape.r;
				tmp_ball.v -= balls[j].v;
				double the_collide_time = collisionDetect(tmp_ball, WallDot(balls[j].shape.p, 1)).first;
				if (DOUBLE_EPS::lt(the_collide_time, ball_collide_time)) {
					ball_collide_time = the_collide_time;
					ball_collide.clear();
					ball_collide.push_back(std::make_pair(i, j));
				}
				else if (DOUBLE_EPS::eq(the_collide_time, ball_collide_time))
					ball_collide.push_back(std::make_pair(i, j));
			}
		return;
	}

	void collision_detect() {
		wall_collide.clear();
		ball_collide.clear();
		wall_collide_time = ball_collide_time = std::numeric_limits<double>::infinity();
		wall_collision_detect();
		ball_collision_detect();
		return;
	}

	void wall_collision_respond() {
		for (auto& the_ball : balls)
			the_ball.integrate(wall_collide_time);
		for (auto& this_wall_collide : wall_collide)
			this_wall_collide.second->collisionRespond(balls[this_wall_collide.first]);
		return;
	}

	void ball_collision_respond() {
		for (auto& the_ball : balls)
			the_ball.integrate(ball_collide_time);
		for (auto& this_ball_collide : ball_collide) {
			Ball& b1 = balls[this_ball_collide.first];
			Ball& b2 = balls[this_ball_collide.second];
			Vector2D base_velocity = b2.v, direction = (b2.shape.p - b1.shape.p).zoomTo(1);
			b1.v -= base_velocity; b2.v = Vector2D(0, 0);
			double t = 2.0 * b1.m / (b1.m + b2.m) * (direction * b1.v);
			b2.v = t * direction;
			b1.v = b1.v - b2.m / b1.m * t * direction;
			b1.v += base_velocity; b2.v += base_velocity;
		}
		return;
	}

	void collision_respond(double& dt_remaining) {
		if (min(wall_collide_time, ball_collide_time) > dt_remaining) {
			for (auto& the_ball : balls)
				the_ball.integrate(dt_remaining);
			dt_remaining = 0;
			return;
		}
		if (wall_collide_time < ball_collide_time)
			wall_collision_respond();
		else ball_collision_respond();
		dt_remaining -= min(wall_collide_time, ball_collide_time);
		return;
	}

	void advance(double dt) {
		double time_step_remaining = dt;
		while (DOUBLE_EPS::gt(time_step_remaining, 0)) {
			if (GetAsyncKeyState(VK_UP) & 0x8000) balls[0].addForce(Vector2D(0, -500) * balls[0].m);
			if (GetAsyncKeyState(VK_DOWN) & 0x8000) balls[0].addForce(Vector2D(0, 120) * balls[0].m);
			if (GetAsyncKeyState(VK_LEFT) & 0x8000) balls[0].addForce(Vector2D(-120, 0) * balls[0].m);
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000) balls[0].addForce(Vector2D(120, 0) * balls[0].m);
			for (auto& the_ball : balls) the_ball.addForce(g * the_ball.m);
			collision_detect();
			collision_respond(time_step_remaining);
		}
		return;
	}

public:
	void main() {
		std::pair<int, int> window_size = createWin(-1, -1, -1, -1, true);

		BeginBatchDraw();
		cleardevice();
		setCurrentColor(Color(0, 200, 200));
		FlushBatchDraw();
		printf("%d %d\n", window_size.first, window_size.second);

		initialize();
		draw();

		double real_lst_time = (double)clock() / CLOCKS_PER_SEC, real_cur_time, // draw time
			time_passed = 0, // simulation time
			speed_rate = 1.0; // draw speed
		bool drew = false;
		while (!(GetAsyncKeyState(VK_F12) & 0x8000)) {
			if (time_passed < draw_time_step) {
				advance(simulation_time_step);
				time_passed += simulation_time_step;
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

		return;
	}
};