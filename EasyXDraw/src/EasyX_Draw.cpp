#include "EasyX_Draw.h"
#include <utility>

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

static COLORREF* pixelColors;
static std::pair<int, int> window_size;

std::pair<int, int> createWin(int x, int y, int width, int height, bool show_console)
{
	HWND hwnd;
	if ((~width) && (~height)) {
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

void drawLine(Vector2D a, Vector2D b, double width, Color color)
{

	return;
}

void drawCircle(Vector2D p, double r, Color color)
{
}

void drawSolidCircle(Vector2D p, double r, Color color)
{
}

void drawDot(Vector2D p, double width, Color color)
{
}
