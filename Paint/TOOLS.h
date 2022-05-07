#pragma once
class TOOLS
{
public:
	static constexpr int NONE = -1;
	static constexpr int PENCIL = 0; // 铅笔
	static constexpr int LINE = 1;  // 画线
	static constexpr int RECTANGLE = 2;  // 矩形
	static constexpr int CIRCLE = 3;  // 圆
	static constexpr int FILL = 4;  // 填充
	static constexpr int ERASER = 5;  // 擦除
	static constexpr int EYEDROPPER = 6;  // 取色器
	static constexpr int MOVEUP = 7;  // 上移
	static constexpr int MOVEDOWN = 8;  // 下移
	static constexpr int MOVELEFT = 9;  // 左移
	static constexpr int MOVERIGHT = 10;  // 右移
	static constexpr int ZOOMIN = 11;  // 放大
	static constexpr int ZOOMOUT = 12;  // 缩小
	static constexpr int CONTRAROTATE = 13; // 逆时针旋转
	static constexpr int ROTATE = 14; // 顺时针旋转
	static constexpr int XSYMMETRY = 15;  // 关于X轴对称
	static constexpr int YSYMMETRY = 16;  // 关于Y轴对称
	static constexpr int OSYMMETRY = 17;  // 关于原点对称
	static constexpr int XSHEAR = 18;  // X错切
	static constexpr int YSHEAR = 19;  // Y错切
	static constexpr int CHOOSE = 20; // 选择
	
	static void AntiAliasing(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF c1, COLORREF c2);

	static void Rectangle(CDC* pDC, CPoint p1, CPoint p2, COLORREF color);

	static void Circle(CDC* pDC, CPoint p1, CPoint p2, COLORREF color);

	static void Fill(CDC* pDC, int x, int y, COLORREF oldcolor, COLORREF newcolor, int height, int width, PBYTE pBits);
	static void Fill(CDC* pDC, CPoint point, COLORREF oldcolor, COLORREF newcolor, int height, int width, PBYTE pBits);
};

