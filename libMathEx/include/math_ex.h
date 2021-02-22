#pragma once

extern const double pi;

namespace DOUBLE_EPS {
	extern const double eps;
	bool eq(double a, double b);
	bool le(double a, double b);
	bool ge(double a, double b);
	bool leq(double a, double b);
	bool geq(double a, double b);
}