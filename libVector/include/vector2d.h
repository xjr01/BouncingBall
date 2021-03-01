#pragma once

class Vector2D {
public:
	double x, y;
	Vector2D() = default;
	Vector2D(double x, double y);
	Vector2D(const Vector2D& t);
	Vector2D& operator = (const Vector2D& t);

	Vector2D operator + (const Vector2D& t) const;
	Vector2D& operator += (const Vector2D& t);
	Vector2D operator - (const Vector2D& t) const;
	Vector2D& operator -= (const Vector2D& t);
	Vector2D operator * (const double& t) const;
	Vector2D& operator *= (const double& t);
	friend Vector2D operator * (const double& v, const Vector2D& t);
	Vector2D operator / (const double& t) const;
	Vector2D& operator /= (const double& t);
	double operator * (const Vector2D& t) const; // dot
	double operator ^ (const Vector2D& t) const; // cross

	bool operator == (const Vector2D& t) const;

	Vector2D zoomTo(double length) const;
	Vector2D& zoomTo_(double length);
	Vector2D scale(double sx, double sy) const;
	Vector2D& scale_(double sx, double sy);
	Vector2D rotate(double angle) const;
	Vector2D& rotate_(double angle);
	Vector2D rotate_degree(double angle_degree) const;
	Vector2D& rotate_degree_(double angle_degree);
	Vector2D shear(double hx, double hy) const;
	Vector2D& shear_(double hx, double hy);

	double length() const;
	friend double abs(const Vector2D& t);
};