#pragma once

#include <cmath>
#include <numeric>

extern const double pi;

namespace DOUBLE_EPS {
	const double eps = 1e-3;
	bool eq(double a, double b);
	bool neq(double a, double b);
	bool lt(double a, double b);
	bool gt(double a, double b);
	bool leq(double a, double b);
	bool geq(double a, double b);
	bool in_between_closed(double x, double l, double r); // true if x is between l and r
	bool in_between_open(double x, double l, double r); // true if x is between l and r
};