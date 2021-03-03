#include "EasyX_Draw.h"
#include <math_ex.h>
#include <utility>
#include <vector>

#undef min
#undef max

Color::Color(int r, int g, int b, double alpha) : r(r), g(g), b(b), alpha(alpha)
{
}

Color::Color(COLORREF color, double alpha) : r(GetRValue(color)), g(GetGValue(color)), b(GetBValue(color)), alpha(alpha)
{
}

Color::operator COLORREF() const
{
	return RGB(r, g, b);
}

COLORREF Color::MixWith(COLORREF color) const
{
	return RGB(
		r * alpha + GetRValue(color) * (1.0 - alpha),
		g * alpha + GetGValue(color) * (1.0 - alpha),
		b * alpha + GetBValue(color) * (1.0 - alpha)
	);
}

COLORREF Color::operator+(const COLORREF& color) const
{
	return MixWith(color);
}

COLORREF operator+(const COLORREF& color, const Color& ths)
{
	return ths.MixWith(color);
}

COLORREF& operator+=(COLORREF& color, const Color& ths)
{
	return color = ths.MixWith(color);
}

static COLORREF* pixelColors;
static std::pair<int, int> window_size;

COLORREF& PIX(int x, int y) {
	return pixelColors[y*window_size.first + x];
}

std::pair<int, int> createWin(int x, int y, int width, int height, bool show_console)
{
	HWND hwnd;
	if (!(~width) && !(~height)) {
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
		if (show_console) initgraph(width, height, SHOWCONSOLE);
		else initgraph(width, height);
		hwnd = GetHWnd();
		SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) - WS_CAPTION);
		SetWindowPos(hwnd, HWND_TOP, 0, 0, width, height, SWP_SHOWWINDOW);
	}
	else {
		if (show_console) initgraph(width, height, SHOWCONSOLE);
		else initgraph(width, height);
		hwnd = GetHWnd();
		SetWindowPos(hwnd, HWND_TOP, x, y, width, height, SWP_SHOWWINDOW);
	}
	pixelColors = GetImageBuffer();
	return window_size = std::make_pair(width, height);
}

template <class T>
void fitInRange_(T& x, T l, T r) {
	x = std::max(x, l); x = std::min(x, r);
	return;
}

template <class T>
T fitInRange(T x, T l, T r) {
	return std::min(std::max(x, l), r);
}

Color cur_color;

void setCurrentColor(Color color)
{
	cur_color = color;
	return;
}

void drawLine(Segment seg, double width, Color color)
{
	int l = fitInRange((int)(std::min(seg.p1.x, seg.p2.x) - width - 1.0), 0, window_size.first - 1),
		r = fitInRange((int)(std::max(seg.p1.x, seg.p2.x) + width + 1.0), 0, window_size.first - 1),
		u = fitInRange((int)(std::min(seg.p1.y, seg.p2.y) - width - 1.0), 0, window_size.second - 1),
		d = fitInRange((int)(std::max(seg.p1.y, seg.p2.y) + width + 1.0), 0, window_size.second - 1);
	for (int y = u; y <= d; ++y)
		for (int x = l; x <= r; ++x)
			if (DOUBLE_EPS::leq(seg.distance(Vector2D(x, y)), width)) {
				PIX(x, y) += color;
			}
	return;
}

void drawLine(Vector2D a, Vector2D b, double width, Color color)
{
	return drawLine(Segment(a, b), width, color);
}

void drawCircle(Circle cir, double width, Color color)
{
	int l = fitInRange((int)(cir.p.x - cir.r - width - 1.0), 0, window_size.first - 1),
		r = fitInRange((int)(cir.p.x + cir.r + width + 1.0), 0, window_size.first - 1),
		u = fitInRange((int)(cir.p.y - cir.r - width - 1.0), 0, window_size.second - 1),
		d = fitInRange((int)(cir.p.y + cir.r + width + 1.0), 0, window_size.second - 1);
	for (int y = u; y <= d; ++y)
		for (int x = l; x <= r; ++x)
			if (DOUBLE_EPS::leq(abs((cir.p - Vector2D(x, y)).length() - cir.r), width)) {
				PIX(x, y) += color;
			}
	return;
}

void drawCircle(Vector2D p, double r, double width, Color color)
{
	return drawCircle(Circle(p, r), width, color);
}

void drawSolidCircle(Circle cir, Color color)
{
	int l = fitInRange((int)(cir.p.x - cir.r - 1.0), 0, window_size.first - 1),
		r = fitInRange((int)(cir.p.x + cir.r + 1.0), 0, window_size.first - 1),
		u = fitInRange((int)(cir.p.y - cir.r - 1.0), 0, window_size.second - 1),
		d = fitInRange((int)(cir.p.y + cir.r + 1.0), 0, window_size.second - 1);
	for (int y = u; y <= d; ++y)
		for (int x = l; x <= r; ++x)
			if (DOUBLE_EPS::leq((cir.p - Vector2D(x, y)).length(), cir.r)) {
				PIX(x, y) += color;
			}
}

void drawSolidCircle(Vector2D p, double r, Color color)
{
	return drawSolidCircle(Circle(p, r), color);
}

void drawDot(Vector2D p, double width, Color color)
{
	return drawSolidCircle(p, width, color);
}

void drawPolygon(PolygonClass poly, double width, Color color)
{
	if (poly.vertices.empty()) return;
	std::vector<Vector2D>::iterator it = poly.vertices.begin(), lst;
	for (lst = it++; it != poly.vertices.end(); lst = it++)
		drawLine(*lst, *it, width);
	it = poly.vertices.begin();
	drawLine(*lst, *it, width);
	return;
}

void drawSolidPolygon(PolygonClass poly, Color color)
{
	if (poly.vertices.size() < 3) return;
	double l, r, u, d;
	l = r = poly.vertices[0].x;
	u = d = poly.vertices[0].y;
	for (std::vector<Vector2D>::iterator it = poly.vertices.begin(); it != poly.vertices.end(); ++it) {
		l = std::min(l, it->x); r = std::max(r, it->x);
		u = std::min(u, it->y); d = std::max(d, it->y);
	}
	l -= 1.0; r += 1.0;
	u -= 1.0; d += 1.0;
	for (int y = (int)u; y <= (int)d; ++y)
		for (int x = (int)l; x <= (int)r; ++x)
			if (poly.inside(Vector2D(x, y), 3)) {
				PIX(x, y) += color;
			}
	return;
}
