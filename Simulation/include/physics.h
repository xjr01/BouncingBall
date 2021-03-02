#pragma once

#include <vector2d.h>
#include <geometry2d.h>

class Ball {
private:
	Circle shape;
	Vector2D v;
	double m;
public:
	Ball() = default;
	Ball(Circle shape, Vector2D v = Vector2D(0, 0), double m = 1);
};