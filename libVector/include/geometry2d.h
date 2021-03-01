#pragma once

#include "vector2d.h"
#include <utility>

class Line {
private:
	Vector2D p, v; // Line: p+tv
public:
	Line(Vector2D p, Vector2D v, bool double_point = false);
	// double_point = true indicates Creating a line with two points
	Line(const Line& t) = default;
	Line& operator = (const Line& t) = default;

	Vector2D cross(const Line& l) const;
	double distance(const Vector2D& x) const;
};

class Segment {
private:
	Line line;
public:
	Vector2D p1, p2;
	Segment(Vector2D p1, Vector2D p2);
	Segment(const Segment& t) = default;
	Segment& operator = (const Segment& t) = default;

	double distance(const Vector2D& x) const;
};

class Circle {
public:
	Vector2D p;
	double r;
	Circle() = default;
	Circle(Vector2D p, double r);
	Circle& operator = (const Circle& t) = default;
};