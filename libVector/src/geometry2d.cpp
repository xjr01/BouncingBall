#include "geometry2d.h"
#include <math_ex.h>
#include <random>

static std::default_random_engine random_engine;

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

bool Line::crossed(const Segment& seg) const
{
	return seg.crossed(*this);
}

Line Line::rotate(double angle) const
{
	return Line(p, v.rotate(angle));
}

Line& Line::rotate_(double angle)
{
	v.rotate_(angle);
	return *this;
}

Line Line::rotate_degree(double angle_degree) const
{
	return rotate(angle_degree / 180.0 * pi);
}

Line& Line::rotate_degree_(double angle_degree)
{
	return rotate_(angle_degree / 180.0 * pi);
}

Segment::Segment(Vector2D p1, Vector2D p2) : p1(p1), p2(p2), line(p1, p2, true)
{
}

Line Segment::get_line() const
{
	return line;
}

double Segment::distance(const Vector2D& x) const
{
	double d1 = (x - p1).length(), d2 = (x - p2).length(),
		d3 = line.distance(x),
		t = line.project_t(x);
	if (DOUBLE_EPS::in_between_closed(t, line.project_t(p1), line.project_t(p2)))
		return std::min(std::min(d1, d2), d3);
	return std::min(d1, d2);
}

bool Segment::crossed(const Line& l) const
{
	double l_t = line.cross_t(l), t1 = line.project_t(p1), t2 = line.project_t(p2);
	return DOUBLE_EPS::in_between_closed(l_t, t1, t2);
}

Circle::Circle(Vector2D p, double r) : p(p), r(r)
{
}

PolygonClass::PolygonClass(std::vector<Vector2D> vertices) : vertices(vertices)
{
}

bool PolygonClass::inside(Vector2D p, int kase) const
{
	Line test_ray(p, Vector2D(1, 0));
	std::uniform_real_distribution<double> uniform(0, pi * 2);
	int tot_in = 0, tot_out = 0;

	while (kase--) {
		test_ray.rotate_(uniform(random_engine));
		std::vector<Vector2D>::const_iterator it = vertices.cbegin(), lst;
		Segment edge;
		bool is_in = 0;

		for (lst = it++; it != vertices.cend(); lst = it++) {
			edge = Segment(*lst, *it);
			if (edge.crossed(test_ray))
				is_in ^= DOUBLE_EPS::gt(test_ray.cross_t(edge.get_line()), 0);
		}
		it = vertices.cbegin();
		edge = Segment(*lst, *it);
		if (edge.crossed(test_ray))
			is_in ^= DOUBLE_EPS::gt(test_ray.cross_t(edge.get_line()), 0);

		tot_in += is_in;
		tot_out += !is_in;
	}

	return tot_in > tot_out;
}
