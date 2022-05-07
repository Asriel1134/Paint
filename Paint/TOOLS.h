#pragma once
class TOOLS
{
public:
	static constexpr int NONE = -1;
	static constexpr int PENCIL = 0; // Ǧ��
	static constexpr int LINE = 1;  // ����
	static constexpr int RECTANGLE = 2;  // ����
	static constexpr int CIRCLE = 3;  // Բ
	static constexpr int FILL = 4;  // ���
	static constexpr int ERASER = 5;  // ����
	static constexpr int EYEDROPPER = 6;  // ȡɫ��
	static constexpr int MOVEUP = 7;  // ����
	static constexpr int MOVEDOWN = 8;  // ����
	static constexpr int MOVELEFT = 9;  // ����
	static constexpr int MOVERIGHT = 10;  // ����
	static constexpr int ZOOMIN = 11;  // �Ŵ�
	static constexpr int ZOOMOUT = 12;  // ��С
	static constexpr int CONTRAROTATE = 13; // ��ʱ����ת
	static constexpr int ROTATE = 14; // ˳ʱ����ת
	static constexpr int XSYMMETRY = 15;  // ����X��Գ�
	static constexpr int YSYMMETRY = 16;  // ����Y��Գ�
	static constexpr int OSYMMETRY = 17;  // ����ԭ��Գ�
	static constexpr int XSHEAR = 18;  // X����
	static constexpr int YSHEAR = 19;  // Y����
	static constexpr int CHOOSE = 20; // ѡ��
	
	static void AntiAliasing(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF c1, COLORREF c2);

	static void Rectangle(CDC* pDC, CPoint p1, CPoint p2, COLORREF color);

	static void Circle(CDC* pDC, CPoint p1, CPoint p2, COLORREF color);

	static void Fill(CDC* pDC, int x, int y, COLORREF oldcolor, COLORREF newcolor, int height, int width, PBYTE pBits);
	static void Fill(CDC* pDC, CPoint point, COLORREF oldcolor, COLORREF newcolor, int height, int width, PBYTE pBits);
};

