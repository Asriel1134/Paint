#include "pch.h"
#include "TOOLS.h"
#include <stack>
#define Round(d) int(d+0.5)

void TOOLS::AntiAliasing(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF c1, COLORREF c2)
{
	float dx, dy, k, x, y;
	dx = (float)(x2 - x1), dy = (float)(y2 - y1);
	k = dy / dx;//求出斜率
	float e = 0;
	y = y1;
	x = x1;
	int red, green, blue;
	BYTE red1 = GetRValue(c1);
	BYTE green1 = GetGValue(c1);
	BYTE blue1 = GetBValue(c1);
	BYTE red2 = GetRValue(c2);
	BYTE green2 = GetGValue(c2);
	BYTE blue2 = GetBValue(c2);

	// 右下
	if (0 <= k && k <= 1 && dx>0)
	{
		for (int x = x1; x <= x2; x++)
		{
			float t = (float)(x - x1) / (x2 - x1);
			red = Round((1 - t) * red1 + t * red2);
			green = Round((1 - t) * green1 + t * green2);
			blue = Round((1 - t) * blue1 + t * blue2);
			int r1 = Round(e * (255 - red) + red);
			int g1 = Round(e * (255 - green) + green);
			int b1 = Round(e * (255 - blue) + blue);
			int r2 = Round((1 - e) * (255 - red) + red);
			int g2 = Round((1 - e) * (255 - green) + green);
			int b2 = Round((1 - e) * (255 - blue) + blue);
			pDC->SetPixel(Round(x), Round(y), RGB(r1, g1, b1));
			pDC->SetPixel(Round(x), Round(y + 1), RGB(r2, g2, b2));
			e = e + k;
			if (e >= 1.0)
			{
				y++;
				e--;
			}
		}
	} 
	else if (k > 1 && dx > 0)
	{
		for (int y = y1; y <= y2; y++)
		{
			float t = (float)(y - y1) / (y2 - y1);
			red = Round((1 - t) * red1 + t * red2);
			green = Round((1 - t) * green1 + t * green2);
			blue = Round((1 - t) * blue1 + t * blue2);
			int r1 = Round(e * (255 - red) + red);
			int g1 = Round(e * (255 - green) + green);
			int b1 = Round(e * (255 - blue) + blue);
			int r2 = Round((1 - e) * (255 - red) + red);
			int g2 = Round((1 - e) * (255 - green) + green);
			int b2 = Round((1 - e) * (255 - blue) + blue);
			pDC->SetPixel(Round(x), Round(y), RGB(r1, g1, b1));
			pDC->SetPixel(Round(x+1), Round(y), RGB(r2, g2, b2));
			e = e + 1/k;
			if (e >= 1.0)
			{
				x++;
				e--;
			}
		}
	}

	// 左上
	else if (0 <= k && k <= 1 && dx < 0)
	{
		for (int x = x1; x >= x2; x--)
		{
			float t = (float)(x - x1) / (x2 - x1);
			red = Round((1 - t) * red1 + t * red2);
			green = Round((1 - t) * green1 + t * green2);
			blue = Round((1 - t) * blue1 + t * blue2);
			int r1 = Round(e * (255 - red) + red);
			int g1 = Round(e * (255 - green) + green);
			int b1 = Round(e * (255 - blue) + blue);
			int r2 = Round((1 - e) * (255 - red) + red);
			int g2 = Round((1 - e) * (255 - green) + green);
			int b2 = Round((1 - e) * (255 - blue) + blue);
			pDC->SetPixel(Round(x), Round(y), RGB(r1, g1, b1));
			pDC->SetPixel(Round(x), Round(y - 1), RGB(r2, g2, b2));
			e = e + k;
			if (e >= 1.0)
			{
				y--;
				e--;
			}
		}
	}
	else if (k > 1 && dx < 0)
	{
		for (int y = y1; y >= y2; y--)
		{
			float t = (float)(y - y1) / (y2 - y1);
			red = Round((1 - t) * red1 + t * red2);
			green = Round((1 - t) * green1 + t * green2);
			blue = Round((1 - t) * blue1 + t * blue2);
			int r1 = Round(e * (255 - red) + red);
			int g1 = Round(e * (255 - green) + green);
			int b1 = Round(e * (255 - blue) + blue);
			int r2 = Round((1 - e) * (255 - red) + red);
			int g2 = Round((1 - e) * (255 - green) + green);
			int b2 = Round((1 - e) * (255 - blue) + blue);
			pDC->SetPixel(Round(x), Round(y), RGB(r1, g1, b1));
			pDC->SetPixel(Round(x - 1), Round(y), RGB(r2, g2, b2));
			e = e + 1 / k;
			if (e >= 1.0)
			{
				x--;
				e--;
			}
		}
	}

	// 右上
	else if (0 >= k && k >= -1 && dx > 0)
	{
		for (int x = x1; x <= x2; x++)
		{
			float t = (float)(x - x1) / (x2 - x1);
			red = Round((1 - t) * red1 + t * red2);
			green = Round((1 - t) * green1 + t * green2);
			blue = Round((1 - t) * blue1 + t * blue2);
			int r1 = Round(e * (255 - red) + red);
			int g1 = Round(e * (255 - green) + green);
			int b1 = Round(e * (255 - blue) + blue);
			int r2 = Round((1 - e) * (255 - red) + red);
			int g2 = Round((1 - e) * (255 - green) + green);
			int b2 = Round((1 - e) * (255 - blue) + blue);
			pDC->SetPixel(Round(x), Round(y), RGB(r1, g1, b1));
			pDC->SetPixel(Round(x), Round(y - 1), RGB(r2, g2, b2));
			e = e - k;
			if (e >= 1.0)
			{
				y--;
				e--;
			}
		}
	}
	else if (k < -1 && dx > 0)
	{
		for (int y = y1; y >= y2; y--)
		{
			float t = (float)(y - y1) / (y2 - y1);
			red = Round((1 - t) * red1 + t * red2);
			green = Round((1 - t) * green1 + t * green2);
			blue = Round((1 - t) * blue1 + t * blue2);
			int r1 = Round(e * (255 - red) + red);
			int g1 = Round(e * (255 - green) + green);
			int b1 = Round(e * (255 - blue) + blue);
			int r2 = Round((1 - e) * (255 - red) + red);
			int g2 = Round((1 - e) * (255 - green) + green);
			int b2 = Round((1 - e) * (255 - blue) + blue);
			pDC->SetPixel(Round(x), Round(y), RGB(r1, g1, b1));
			pDC->SetPixel(Round(x + 1), Round(y), RGB(r2, g2, b2));
			e = e - 1 / k;
			if (e >= 1.0)
			{
				x++;
				e--;
			}
		}
	}

	// 左下
	else if (0 >= k && k >= -1 && dx < 0)
	{
	for (int x = x1; x >= x2; x--)
	{
		float t = (float)(x - x1) / (x2 - x1);
		red = Round((1 - t) * red1 + t * red2);
		green = Round((1 - t) * green1 + t * green2);
		blue = Round((1 - t) * blue1 + t * blue2);
		int r1 = Round(e * (255 - red) + red);
		int g1 = Round(e * (255 - green) + green);
		int b1 = Round(e * (255 - blue) + blue);
		int r2 = Round((1 - e) * (255 - red) + red);
		int g2 = Round((1 - e) * (255 - green) + green);
		int b2 = Round((1 - e) * (255 - blue) + blue);
		pDC->SetPixel(Round(x), Round(y), RGB(r1, g1, b1));
		pDC->SetPixel(Round(x), Round(y + 1), RGB(r2, g2, b2));
		e = e - k;
		if (e >= 1.0)
		{
			y++;
			e--;
		}
	}
	}
	else if (k < -1 && dx < 0)
	{
		for (int y = y1; y <= y2; y++)
		{
			float t = (float)(y - y1) / (y2 - y1);
			red = Round((1 - t) * red1 + t * red2);
			green = Round((1 - t) * green1 + t * green2);
			blue = Round((1 - t) * blue1 + t * blue2);
			int r1 = Round(e * (255 - red) + red);
			int g1 = Round(e * (255 - green) + green);
			int b1 = Round(e * (255 - blue) + blue);
			int r2 = Round((1 - e) * (255 - red) + red);
			int g2 = Round((1 - e) * (255 - green) + green);
			int b2 = Round((1 - e) * (255 - blue) + blue);
			pDC->SetPixel(Round(x), Round(y), RGB(r1, g1, b1));
			pDC->SetPixel(Round(x - 1), Round(y), RGB(r2, g2, b2));
			e = e - 1 / k;
			if (e >= 1.0)
			{
				x--;
				e--;
			}
		}
	}
	else if (dx == 0 && dy > 0) {
		for (int y = y1; y <= y2; y++)
		{
			float t = (float)(y - y1) / (y2 - y1);
			red = Round((1 - t) * red1 + t * red2);
			green = Round((1 - t) * green1 + t * green2);
			blue = Round((1 - t) * blue1 + t * blue2);
			pDC->SetPixel(Round(x), Round(y), RGB(red, green, blue));
		}
	}
	else if (dx == 0 && dy < 0) {
		for (int y = y1; y >= y2; y--)
		{
			float t = (float)(y - y1) / (y2 - y1);
			red = Round((1 - t) * red1 + t * red2);
			green = Round((1 - t) * green1 + t * green2);
			blue = Round((1 - t) * blue1 + t * blue2);
			pDC->SetPixel(Round(x), Round(y), RGB(red, green, blue));
		}
	}
}

void TOOLS::Rectangle(CDC* pDC, CPoint p1, CPoint p2, COLORREF color)
{
	CPen pen(PS_SOLID, 1, color);
	pDC->SelectObject(&pen);
	pDC->MoveTo(p1.x, p1.y);
	pDC->LineTo(p1.x, p2.y);
	pDC->LineTo(p2.x, p2.y);
	pDC->LineTo(p2.x, p1.y);
	pDC->LineTo(p1.x, p1.y);
}

void TOOLS::Circle(CDC* pDC, CPoint p1, CPoint p2, COLORREF color)
{
	int a = abs(p1.x - p2.x) / 2;
	int b = abs(p1.y - p2.y) / 2;
	int x0 = (p1.x + p2.x) / 2;
	int y0 = (p1.y + p2.y) / 2;

	int x = 0, y = b;
	pDC->SetPixel(x0 + x, y0 + y, color);
	pDC->SetPixel(x0 - x, y0 + y, color);
	pDC->SetPixel(x0 + x, y0 - y, color);
	pDC->SetPixel(x0 - x, y0 - y, color);

	//上下部分
	double d1 = b * b + a * a * (-b + 0.25);
	while (b * b * x < a * a * y)
	{
		if (d1 < 0)
		{
			d1 += b * b * (2 * x + 3);
			x++;
		}
		else
		{
			d1 += (b * b * (2 * x + 3) + a * a * (-2 * y + 2));
			x++; y--;
		}

		pDC->SetPixel(x0 + x, y0 + y, color);
		pDC->SetPixel(x0 - x, y0 + y, color);
		pDC->SetPixel(x0 + x, y0 - y, color);
		pDC->SetPixel(x0 - x, y0 - y, color);
	}
	//左右部分
	double d2 = sqrt((double)b * (x + 0.5)) + sqrt((double)a * (y - 1)) - sqrt((double)a * b);
	while (y > 0)
	{
		if (d2 < 0)
		{
			d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
			x++; y--;
		}
		else
		{
			d2 += a * a * (-2 * y + 3);
			y--;
		}
		//draw_pixel_4(pDC,x0,y0,x,y,color);
		pDC->SetPixel(x0 + x, y0 + y, color);
		pDC->SetPixel(x0 - x, y0 + y, color);
		pDC->SetPixel(x0 + x, y0 - y, color);
		pDC->SetPixel(x0 - x, y0 - y, color);
	}
}

void TOOLS::Fill(CDC* pDC, int x, int y, COLORREF oldcolor, COLORREF newcolor, int height, int width, PBYTE pBits)
{
	if (newcolor != oldcolor) {
		int position = (width * y + x) * 4;

		if (x < 0 || x >= width || y < 0 || y >= height)
			return;
		if (pBits[position] == GetBValue(oldcolor) && pBits[position + 1] == GetGValue(oldcolor) && pBits[position + 2] == GetRValue(oldcolor))
		{
			pBits[position] = GetBValue(newcolor);
			pBits[position + 1] = GetGValue(newcolor);
			pBits[position + 2] = GetRValue(newcolor);
			Fill(pDC, x, y + 1, oldcolor, newcolor, height, width, pBits);
			Fill(pDC, x, y - 1, oldcolor, newcolor, height, width, pBits);
			Fill(pDC, x - 1, y, oldcolor, newcolor, height, width, pBits);
			Fill(pDC, x + 1, y, oldcolor, newcolor, height, width, pBits);
		}
	}
}

void TOOLS::Fill(CDC* pDC, CPoint point, COLORREF oldcolor, COLORREF newcolor, int height, int width, PBYTE pBits) {
	// https://blog.csdn.net/AHcola233/article/details/118092066
	std::stack<CPoint> fillStack;
	CPoint currentPoint;
	BYTE oleRed = GetRValue(oldcolor);
	BYTE oleGreen = GetGValue(oldcolor);
	BYTE oleBlue = GetBValue(oldcolor);
	BYTE newRed = GetRValue(newcolor);
	BYTE newGreen = GetGValue(newcolor);
	BYTE newBlue = GetBValue(newcolor);

	fillStack.push(point);
	while (!fillStack.empty()) {
		bool reachLeft = false;
		bool reachRight = false;
		currentPoint = fillStack.top();
		fillStack.pop();
		while (currentPoint.y >0 && pDC->GetPixel(currentPoint.x, currentPoint.y - 1) == oldcolor) {
			currentPoint.y--;
		}
		while (currentPoint.y <= height && pDC->GetPixel(currentPoint.x, currentPoint.y) == oldcolor) {
			if (reachLeft && pDC->GetPixel(currentPoint.x - 1, currentPoint.y) != oldcolor)
				reachLeft = false;
			if (reachRight && pDC->GetPixel(currentPoint.x + 1, currentPoint.y) != oldcolor)
				reachRight = false;
			pDC->SetPixel(currentPoint, newcolor);
			if (currentPoint.x > 0 && pDC->GetPixel(currentPoint.x - 1, currentPoint.y) == oldcolor && !reachLeft) {
				fillStack.push(CPoint(currentPoint.x - 1, currentPoint.y));
				reachLeft = true;
			}
			if (currentPoint.x < width && pDC->GetPixel(currentPoint.x + 1, currentPoint.y) == oldcolor && !reachRight) {
				fillStack.push(CPoint(currentPoint.x + 1, currentPoint.y));
				reachRight = true;
			}
			currentPoint.y++;
		}
	}
}