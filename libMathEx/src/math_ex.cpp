#include <cmath>
#include "math_ex.h"

const double pi = acos(-1.0);

const double DOUBLE_EPS::eps = 1e-6;

bool DOUBLE_EPS::eq(double a, double b)
{
	return abs(a - b) < eps;
}

bool DOUBLE_EPS::le(double a, double b)
{
	return a < b && !eq(a, b);
}

bool DOUBLE_EPS::ge(double a, double b)
{
	return a > b && !eq(a, b);
}

bool DOUBLE_EPS::leq(double a, double b)
{
	return a <= b || eq(a, b);
}

bool DOUBLE_EPS::geq(double a, double b)
{
	return a >= b || eq(a, b);
}
