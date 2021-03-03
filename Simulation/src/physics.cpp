#include "physics.h"
#include <math_ex.h>

const double time_step = .01;

Ball::Ball() : shape(), v(), m(), a(Vector2D(0, 0))
{
}

Ball::Ball(Circle shape, Vector2D v, double m) : shape(shape), v(v), m(m), a(Vector2D(0, 0))
{
}

void Ball::set_shape(const Circle& shape)
{
	this->shape = shape;
	return;
}

void Ball::set_v(const Vector2D& v)
{
	this->v = v;
	return;
}

void Ball::set_m(const double& m)
{
	this->m = m;
	return;
}

double Ball::get_mass()
{
	return m;
}

void Ball::addForce(Vector2D force)
{
	this->a += force / m;
	return;
}

void Ball::integrate(double time_step)
{
	shape.p += v * time_step;
	v += a * time_step;
	a = Vector2D(0, 0);
	return;
}

Wall::Wall() : elasticity(1)
{
}

Wall::Wall(double elasticity) : elasticity(elasticity)
{
}

WallDot::WallDot(Vector2D p, double elasticity) : p(p), Wall(elasticity)
{
}

void WallDot::collisionRespond(Ball& ball)
{
	Vector2D n_cap = (ball.shape.p - p).zoomTo(1); // unit normal vector
	Vector2D vn = (ball.v * n_cap) * n_cap,
		vt = ball.v - vn;
	vn *= -elasticity;
	ball.v = vn + vt;
	return;
}

WallLine::WallLine(Line line, double elasticity) : line(line), Wall(elasticity)
{
}

void WallLine::collisionRespond(Ball& ball)
{
	Vector2D project = line.project(ball.shape.p), n_cap = (ball.shape.p - project).zoomTo(1);
	Vector2D vn = (ball.v * n_cap) * n_cap,
		vt = ball.v - vn;
	vn *= -elasticity;
	ball.v = vn + vt;
	return;
}

WallSegment::WallSegment(Segment seg, double elasticity) : seg(seg), Wall(elasticity)
{
}

void WallSegment::collisionRespond(Ball& ball)
{
	return;
}

std::pair<double, std::shared_ptr<Wall>> collisionDetect(const Ball& ball, const WallDot& wall)
{
	if (DOUBLE_EPS::eq(ball.v.length(), 0) || (wall.p - ball.shape.p) * ball.v <= 0)
		return std::make_pair(std::numeric_limits<double>::infinity(), std::shared_ptr<Wall>());
	std::vector<double> collision_time = Line(ball.shape.p, ball.v).cross_t(Circle(wall.p, ball.shape.r));
	if (collision_time.empty())
		return std::make_pair(std::numeric_limits<double>::infinity(), std::shared_ptr<Wall>());
	return std::make_pair(std::max(collision_time[0], 0.0), std::make_shared<WallDot>(wall));
}

std::pair<double, std::shared_ptr<Wall>> collisionDetect(const Ball& ball, const WallLine& wall)
{
	Vector2D project = wall.line.project(ball.shape.p);
	if (DOUBLE_EPS::eq(ball.v.length(), 0) || (project - ball.shape.p) * ball.v <= 0)
		return std::make_pair(std::numeric_limits<double>::infinity(), std::shared_ptr<Wall>());
	Line parallel(wall.line.move((ball.shape.p - project).zoomTo(ball.shape.r)));
	double line_collision_time = Line(ball.shape.p, ball.v).cross_t(parallel);
	return std::make_pair(std::max(line_collision_time, 0.0), std::make_shared<WallLine>(wall));
}

std::pair<double, std::shared_ptr<Wall>> collisionDetect(const Ball& ball, const WallSegment& wall)
{
	std::pair<double, std::shared_ptr<Wall>> ans =
		std::make_pair(std::numeric_limits<double>::infinity(), std::shared_ptr<Wall>()), tmp;
	if (DOUBLE_EPS::eq(ball.v.length(), 0))
		return ans;
	Vector2D project = wall.seg.get_line().project(ball.shape.p),
		delta = (ball.shape.p - project).zoomTo(ball.shape.r);
	if (Segment(wall.seg.p1 + delta, wall.seg.p2 + delta).crossed(Line(ball.shape.p, ball.v)))
		ans = collisionDetect(ball, WallLine(wall.seg.get_line()));
	tmp = collisionDetect(ball, WallDot(wall.seg.p1));
	if (tmp.first < ans.first) ans = tmp;
	tmp = collisionDetect(ball, WallDot(wall.seg.p2));
	if (tmp.first < ans.first) ans = tmp;
	return ans;
}
