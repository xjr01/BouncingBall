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
	if (DOUBLE_EPS::eq(l.v ^ v, 0)) throw "Parallel lines";
	double t = (l.v ^ (l.p - p)) / (l.v ^ v);
	return p + t * v;
}

double Line::distance(const Vector2D& x) const
{
	Vector2D crs_ptr = cross(Line(x, v.rotate(pi / 2)));
	return (x - crs_ptr).length();
}

Segment::Segment(Vector2D p1, Vector2D p2) : p1(p1), p2(p2), line(p1, p2, true)
{
}

double Segment::distance(const Vector2D& x) const
{
	double d1 = (x - p1).length(), d2 = (x - p2).length(),
		d3 = line.distance(x);
	return std::min(std::min(d1, d2), d3);
}

Circle::Circle(Vector2D p, double r) : p(p), r(r)
{
}
