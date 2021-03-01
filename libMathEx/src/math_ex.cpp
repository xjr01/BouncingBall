#include <cmath>
#include "math_ex.h"

const double pi = acos(-1.0);

bool DOUBLE_EPS::eq(double a, double b)
{
	return abs(a - b) < eps;
}

bool DOUBLE_EPS::neq(double a, double b)
{
	return !eq(a, b);
}

bool DOUBLE_EPS::lt(double a, double b)
{
	return a < b && neq(a, b);
}

bool DOUBLE_EPS::gt(double a, double b)
{
	return a > b && neq(a, b);
}

bool DOUBLE_EPS::leq(double a, double b)
{
	return a < b || eq(a, b);
}

bool DOUBLE_EPS::geq(double a, double b)
{
	return a > b || eq(a, b);
}
