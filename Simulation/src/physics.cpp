#include "physics.h"

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

void Ball::addForce(Vector2D force)
{
	this->a += force / m;
	return;
}

void Ball::integrate(double time_step)
{
	shape.p += v * time_step;
	v += a * time_step;
	return;
}

WallDot::WallDot(Vector2D p) : p(p)
{
}

WallSegment::WallSegment(Segment seg) : seg(seg)
{
}

std::pair<double, Wall*> collisionDetect(const Ball& ball, const WallDot& wall)
{
	std::vector<double> collision_time = Line(ball.shape.p, ball.v).cross_t(Circle(wall.p, ball.shape.r));
	if (collision_time.empty())
		return std::make_pair(std::numeric_limits<double>::infinity(), (Wall*)NULL);
	return std::make_pair(collision_time[0], (Wall*)&wall);
}

std::pair<double, Wall*> collisionDetect(const Ball& ball, const WallSegment& wall)
{

}
