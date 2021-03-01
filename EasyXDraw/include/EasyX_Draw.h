#pragma once

#include <graphics.h>
#include <vector2d.h>
#include <geometry2d.h>
#include <utility>

class Color {
private:
	int r, g, b;
	double alpha;
public:
	Color() = default;
	Color(int r, int g, int b, double alpha = 1.0);
	Color(COLORREF color, double alpha = 1.0);
	Color(const Color& t) = default;
	Color& operator = (const Color& t) = default;
	~Color() = default;

	operator COLORREF() const; // Ignoring alpha
	COLORREF MixWith(COLORREF color) const;
	COLORREF operator + (const COLORREF& color) const;
	friend COLORREF operator + (const COLORREF& color, const Color& ths);
	friend COLORREF& operator += (COLORREF& color, const Color& ths);
};

std::pair<int, int> createWin(int x = -1, int y = -1, int width = -1, int height = -1, bool show_console = false);

// Coordinates in pixels
extern Color cur_color;
void setCurrentColor(Color color);
void drawLine(Segment seg, double width, Color color = cur_color);
void drawLine(Vector2D a, Vector2D b, double width, Color color = cur_color);
void drawCircle(Circle cir, double width, Color color = cur_color);
void drawCircle(Vector2D p, double r, double width, Color color = cur_color);
void drawSolidCircle(Circle cir, Color color = cur_color);
void drawSolidCircle(Vector2D p, double r, Color color = cur_color);
void drawDot(Vector2D p, double width, Color color = cur_color);
void drawPolygon(PolygonClass poly, double width, Color color = cur_color);
void drawSolidPolygon(PolygonClass poly, Color color = cur_color);
