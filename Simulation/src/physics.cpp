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

WallSegment::WallSegment(Segment seg, double elasticity) : seg(seg), Wall(elasticity)
{
}

void WallSegment::collisionRespond(Ball& ball)
{
	// TODO
	return;
}

std::pair<double, Wall*> collisionDetect(const Ball& ball, const WallDot& wall)
{
	if (DOUBLE_EPS::eq(ball.v.length(), 0) || (wall.p - ball.shape.p) * ball.v <= 0)
		return std::make_pair(std::numeric_limits<double>::infinity(), (Wall*)nullptr);
	std::vector<double> collision_time = Line(ball.shape.p, ball.v).cross_t(Circle(wall.p, ball.shape.r));
	if (collision_time.empty())
		return std::make_pair(std::numeric_limits<double>::infinity(), (Wall*)nullptr);
	return std::make_pair(std::max(collision_time[0], 0.0), (Wall*)&wall);
}

std::pair<double, Wall*> collisionDetect(const Ball& ball, const WallSegment& wall)
{
	// TODO
	return std::make_pair(std::numeric_limits<double>::infinity(), (Wall*)nullptr);
}
