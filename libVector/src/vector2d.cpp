#include "vector2d.h"
#include <math_ex.h>
#include <cmath>

Vector2D::Vector2D(double x, double y) : x(x), y(y)
{
}

Vector2D::Vector2D(const Vector2D& t) : x(t.x), y(t.y)
{
}

Vector2D& Vector2D::operator=(const Vector2D& t)
{
    x = t.x; y = t.y;
    return *this;
}

Vector2D Vector2D::operator+(const Vector2D& t) const
{
    return Vector2D(x + t.x, y + t.y);
}

Vector2D& Vector2D::operator+=(const Vector2D& t)
{
    return *this = *this + t;
}

Vector2D Vector2D::operator-(const Vector2D& t) const
{
    return Vector2D(x - t.x, y - t.y);
}

Vector2D& Vector2D::operator-=(const Vector2D& t)
{
    return *this = *this - t;
}

Vector2D Vector2D::operator*(const double& t) const
{
    return Vector2D(x * t, y * t);
}

Vector2D& Vector2D::operator*=(const double& t)
{
    return *this = *this * t;
}

Vector2D Vector2D::operator/(const double& t) const
{
    return Vector2D(x / t, y / t);
}

Vector2D& Vector2D::operator/=(const double& t)
{
    return *this = *this / t;
}

double Vector2D::operator*(const Vector2D& t) const
{
    return x * t.x + y * t.y;
}

double Vector2D::operator^(const Vector2D& t) const
{
    return x * t.y - y * t.x;
}

bool Vector2D::operator==(const Vector2D& t) const
{
    return DOUBLE_EPS::eq(x, t.x) && DOUBLE_EPS::eq(y, t.y);
}

Vector2D Vector2D::zoomTo(double length) const
{
    return *this / this->length() * length;
}

Vector2D& Vector2D::zoomTo_(double length)
{
    return *this = zoomTo(length);
}

Vector2D Vector2D::scale(double sx, double sy) const
{
    return Vector2D(x * sx, y * sy);
}

Vector2D& Vector2D::scale_(double sx, double sy)
{
    return *this = scale(sx, sy);
}

Vector2D Vector2D::rotate(double angle) const
{
    return Vector2D(
        x * cos(angle) - y * sin(angle),
        x * sin(angle) + y * cos(angle)
    );
}

Vector2D& Vector2D::rotate_(double angle)
{
    return *this = rotate(angle);
}

Vector2D Vector2D::rotate_degree(double angle_degree) const
{
    return rotate(angle_degree / 180.0 * pi);
}

Vector2D& Vector2D::rotate_degree_(double angle_degree)
{
    return *this = rotate_degree(angle_degree);
}

Vector2D Vector2D::rotate_with_center(double angle, Vector2D center) const
{
    return (*this - center).rotate(angle) + center;
}

Vector2D& Vector2D::rotate_with_center_(double angle, Vector2D center)
{
    return *this = this->rotate_with_center(angle, center);
}

Vector2D Vector2D::rotate_with_center_degree(double angle_degree, Vector2D center) const
{
    return rotate_with_center(angle_degree / 180.0 * pi, center);
}

Vector2D& Vector2D::rotate_with_center_degree_(double angle_degree, Vector2D center)
{
    return rotate_with_center_(angle_degree / 180.0 * pi, center);
}

Vector2D Vector2D::shear(double hx, double hy) const
{
    return Vector2D(x + y * hx, x * hy + y);
}

Vector2D& Vector2D::shear_(double hx, double hy)
{
    return *this = shear(hx, hy);
}

double Vector2D::length() const
{
    return sqrt(x * x + y * y);
}

Vector2D operator*(const double& v, const Vector2D& t)
{
    return Vector2D(t.x * v, t.y * v);
}

double abs(const Vector2D& t)
{
    return t.length();
}
