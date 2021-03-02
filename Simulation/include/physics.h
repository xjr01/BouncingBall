#pragma once

#include <vector2d.h>
#include <geometry2d.h>

extern const double time_step; // In seconds

class WallDot;
class WallSegment;

class Ball {
private:
	Circle shape;
	Vector2D v;
	double m;
	Vector2D a;
public:
	Ball();
	Ball(Circle shape, Vector2D v = Vector2D(0, 0), double m = 1);

	void set_shape(const Circle& shape);
	void set_v(const Vector2D& v);
	void set_m(const double& m);

	void addForce(Vector2D force);
	void integrate(double time_step); // call after doing all the collision detect
	friend double collisionDetect(const Ball& ball, const WallDot& wall);
		// returns how many seconds before the collision happens, returns inf if no collision at all
	friend double collisionDetect(const Ball& ball, const WallSegment& wall);
};

class Wall {};

class WallDot : public Wall {
private:
	Vector2D p;
public:
	WallDot() = default;
	explicit WallDot(Vector2D p);

	friend double collisionDetect(const Ball& ball, const WallDot& wall);
};

class WallSegment : public Wall {
private:
	Segment seg;
public:
	WallSegment() = default;
	explicit WallSegment(Segment seg);

	friend double collisionDetect(const Ball& ball, const WallSegment& wall);
};