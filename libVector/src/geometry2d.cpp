#include "geometry2d.h"
#include <math_ex.h>

Line::Line(Vector2D p, Vector2D v, bool double_point)
{
	if (double_point) {
		this->p = p;
		this->v = (v - p).zoomTo(1);
	}
	else {
		this->p = p;
		this->v = v;
	}
}

Vector2D Line::cross(const Line& l) const
{
	return p + cross_t(l) * v;
}

Vector2D Line::project(const Vector2D& x) const
{
	return p + project_t(x) * v;
}

double Line::cross_t(const Line& l) const
{
	if ((l.v ^ v) == 0) throw "Parallel lines";
	double t = (l.v ^ (l.p - p)) / (l.v ^ v);
	return t;
}

double Line::project_t(const Vector2D& x) const
{
	return cross_t(Line(x, v.rotate(pi / 2)));
}

double Line::distance(const Vector2D& x) const
{
	Vector2D crs_ptr = project(x);
	return (x - crs_ptr).length();
}

Segment::Segment(Vector2D p1, Vector2D p2) : p1(p1), p2(p2), line(p1, p2, true)
{
}

double Segment::distance(const Vector2D& x) const
{
	double d1 = (x - p1).length(), d2 = (x - p2).length(),
		d3 = line.distance(x),
		t = line.project_t(x);
	if ((line.project_t(p1) <= t && t <= line.project_t(p2)) || (line.project_t(p2) <= t && t <= line.project_t(p1)))
		return std::min(std::min(d1, d2), d3);
	return std::min(d1, d2);
}

Circle::Circle(Vector2D p, double r) : p(p), r(r)
{
}
