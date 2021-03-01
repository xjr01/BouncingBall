#pragma once

#include <cmath>

extern const double pi;

namespace DOUBLE_EPS {
	const double eps = 1e-6;
	bool eq(double a, double b);
	bool neq(double a, double b);
	bool lt(double a, double b);
	bool gt(double a, double b);
	bool leq(double a, double b);
	bool geq(double a, double b);
};