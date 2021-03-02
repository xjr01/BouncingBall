#pragma once

#include "vector2d.h"
#include <utility>
#include <vector>

class Segment;
class Circle;

class Line {
private:
	Vector2D p, v; // Line: p+tv
public:
	Line() = default;
	Line(Vector2D p, Vector2D v, bool double_point = false);
	// double_point = true indicates Creating a line with two points

	Vector2D cross(const Line& l) const;
	Vector2D project(const Vector2D& x) const;
	double cross_t(const Line& l) const; // returns t where the cross point is p+tv
	double project_t(const Vector2D& x) const;
	double distance(const Vector2D& x) const;
	bool crossed(const Segment& seg) const;
	std::vector<Vector2D> cross(const Circle& c) const;
	std::vector<double> cross_t(const Circle& c) const;

	Line rotate(double angle) const;
	Line& rotate_(double angle);
	Line rotate_degree(double angle_degree) const;
	Line& rotate_degree_(double angle_degree);
};

class Segment {
private:
	Line line;
public:
	Vector2D p1, p2;
	Segment() = default;
	Segment(Vector2D p1, Vector2D p2);

	Line get_line() const;

	double distance(const Vector2D& x) const;
	bool crossed(const Line& l) const;
};

class Circle {
public:
	Vector2D p;
	double r;
	Circle() = default;
	Circle(Vector2D p, double r);

	std::vector<Vector2D> cross(const Line& l) const;
};

class PolygonClass {
public:
	std::vector<Vector2D> vertices;
	PolygonClass() = default;
	PolygonClass(std::vector<Vector2D> vertices);

	bool inside(Vector2D p, int kase = 5) const;
};