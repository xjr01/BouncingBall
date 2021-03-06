#pragma once

#include <vector2d.h>
#include <geometry2d.h>
#include <utility>
#include <memory>

extern void draw();

class Wall;
class WallDot;
class WallLine;
class WallSegment;

class Ball {
private:
	Vector2D a;
public:
	Circle shape;
	Vector2D v;
	double m;
	Ball();
	Ball(Circle shape, Vector2D v = Vector2D(0, 0), double m = 1);

	void addForce(Vector2D force);
	void integrate(double time_step); // call after doing all the collision detect
};

class Wall {
protected:
	double elasticity;
public:
	Wall();
	explicit Wall(double elasticity);
	virtual void collisionRespond(Ball& ball) = 0;
};

class WallDot : public Wall {
private:
	Vector2D p;
	double r;
public:
	WallDot() = default;
	explicit WallDot(Vector2D p, double r = 0, double elasticity = .8);

	void collisionRespond(Ball& ball) override;
	friend std::pair<double, std::shared_ptr<Wall>> collisionDetect(const Ball& ball, const WallDot& wall);
		/* returns how many seconds before the collision happens, returns inf if no collision at all
		 * and which part of the wall will be touching the 
		 */
	friend class BouncingBallDriver;
};

class WallLine : public Wall {
private:
	Line line;
public:
	WallLine() = default;
	explicit WallLine(Line line, double elasticity = .8);

	void collisionRespond(Ball& ball) override;
	friend std::pair<double, std::shared_ptr<Wall>> collisionDetect(const Ball& ball, const WallLine& wall);
	friend class BouncingBallDriver;
};

class WallSegment : public Wall {
private:
	Segment seg;
public:
	WallSegment() = default;
	explicit WallSegment(Segment seg, double elasticity = .8);

	void collisionRespond(Ball& ball) override;
	friend std::pair<double, std::shared_ptr<Wall>> collisionDetect(const Ball& ball, const WallSegment& wall);
	friend class BouncingBallDriver;
};