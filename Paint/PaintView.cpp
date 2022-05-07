
// PaintView.cpp: CPaintView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Paint.h"
#endif

#include "PaintDoc.h"
#include "PaintView.h"
#define _USE_MATH_DEFINES
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <iostream>


// CPaintView

IMPLEMENT_DYNCREATE(CPaintView, CView)

BEGIN_MESSAGE_MAP(CPaintView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPaintView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_TOOL_LINE, &CPaintView::OnToolLine)
	ON_COMMAND(ID_TOOL_FOREGROUND, &CPaintView::OnToolForeground)
	ON_COMMAND(ID_TOOL_BACKGROUND, &CPaintView::OnToolBackground)
	ON_COMMAND(ID_REVOKE, &CPaintView::OnRevoke)
	ON_COMMAND(ID_TOOL_PENCIL, &CPaintView::OnToolPencil)
	ON_COMMAND(ID_TOOL_CIRCLE, &CPaintView::OnToolCircle)
	ON_COMMAND(ID_TOOL_RECTANGLE, &CPaintView::OnToolRectangle)
	ON_COMMAND(ID_TOOL_FILL, &CPaintView::OnToolFill)
	ON_WM_NCPAINT()
	ON_COMMAND(ID_TOOL_EYEDROPPER, &CPaintView::OnToolEyedropper)
	ON_COMMAND(ID_TOOL_ERASER, &CPaintView::OnToolEraser)
	ON_COMMAND(ID_TOOL_NONE, &CPaintView::OnToolNone)
	ON_COMMAND(ID_TOOL_CHOOSE, &CPaintView::OnToolChoose)
	ON_COMMAND(ID_TOOL_ZOOMIN, &CPaintView::OnToolZoomin)
	ON_COMMAND(ID_TOOL_ZOOMOUT, &CPaintView::OnToolZoomout)
	ON_COMMAND(ID_TOOL_DOWN, &CPaintView::OnToolDown)
	ON_COMMAND(ID_TOOL_LEFT, &CPaintView::OnToolLeft)
	ON_COMMAND(ID_TOOL_UP, &CPaintView::OnToolUp)
	ON_COMMAND(ID_TOOL_RIGHT, &CPaintView::OnToolRight)
	ON_COMMAND(ID_TOOL_CONTRAROTATE, &CPaintView::OnToolContrarotate)
	ON_COMMAND(ID_TOOL_ROTATE, &CPaintView::OnToolRotate)
	ON_COMMAND(ID_TOOL_XSHEAR, &CPaintView::OnToolXshear)
	ON_COMMAND(ID_TOOL_YSHEAR, &CPaintView::OnToolYshear)
END_MESSAGE_MAP()

// CPaintView 构造/析构

CPaintView::CPaintView() noexcept
{
	// TODO: 在此处添加构造代码
	currentTool = -1;
	currentState = 0;
	moved = false;
	foreground = 0x00000000;
	background = 0x00FFFFFF;
	eraserWidth = 10;
	chooseState = 0;
}

CPaintView::~CPaintView()
{
}

BOOL CPaintView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPaintView 绘图

void CPaintView::OnDraw(CDC* /*pDC*/)
{
	CPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CPaintView 打印


void CPaintView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPaintView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPaintView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPaintView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CPaintView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPaintView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPaintView 诊断

#ifdef _DEBUG
void CPaintView::AssertValid() const
{
	CView::AssertValid();
}

void CPaintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPaintDoc* CPaintView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPaintDoc)));
	return (CPaintDoc*)m_pDocument;
}
#endif //_DEBUG


// CPaintView 消息处理程序


void CPaintView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (chooseState == 0) {
		// ********************  画线/矩形/圆  ********************
		if (currentTool == Tools.LINE || currentTool == Tools.RECTANGLE || currentTool == Tools.CIRCLE) {
			// 确定直线起点
			m_Startp = point;
			// 开始画线
			currentState = 1;
			CDC* pDC = GetDC();
			// 获取客户区大小
			CRect rect;
			GetClientRect(&rect);
			// 创建兼容DC
			m_pMemDC.CreateCompatibleDC(pDC);
			// 为兼容DC创建位图
			m_pMemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
			m_pMemDC.SelectObject(&m_pMemBitmap);
			// 复制客户区DC到兼容DC
			m_pMemDC.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);

			// 保存历史记录
			SaveHistory(&m_pMemBitmap);
			// 释放资源
			ReleaseDC(pDC);
		}

		// ********************  画笔  ********************
		if (currentTool == Tools.PENCIL) {
			currentState = 1;
			SaveHistory();
			m_Startp = point;
		}

		// ********************  橡皮  ********************
		if (currentTool == Tools.ERASER) {
			currentState = 1;
			SaveHistory();
			m_Startp = point;
		}

		// ********************  填充  ********************
		if (currentTool == Tools.FILL) {
			SaveHistory();
		}
	}

	// ********************  选择  ********************
	if (currentTool == Tools.CHOOSE && chooseState == 0) {
		// 确定直线起点
		m_Startp = point;
		// 开始画线
		currentState = 1;
		CDC* pDC = GetDC();
		// 获取客户区大小
		CRect rect;
		GetClientRect(&rect);
		// 创建兼容DC
		m_pMemDC.CreateCompatibleDC(pDC);
		// 为兼容DC创建位图
		m_pMemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		m_pMemDC.SelectObject(&m_pMemBitmap);
		// 复制客户区DC到兼容DC
		m_pMemDC.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);

		m_sMemDC.CreateCompatibleDC(pDC);
		// 为兼容DC创建位图
		m_sMemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		m_sMemDC.SelectObject(&m_sMemBitmap);
		// 复制客户区DC到兼容DC
		m_sMemDC.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);

		m_cMemDC.CreateCompatibleDC(pDC);
		// 为兼容DC创建位图
		m_cMemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		m_cMemDC.SelectObject(&m_cMemBitmap);

		SaveHistory();
		// 释放资源
		ReleaseDC(pDC);
	}
	CView::OnLButtonDown(nFlags, point);
}


void CPaintView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (chooseState == 0) {
		// ********************  画线/矩形/圆  ********************
		if ((currentTool == Tools.LINE || currentTool == Tools.RECTANGLE || currentTool == Tools.CIRCLE) && currentState == 1) {
			// 防止起点和终点相同出现问题
			moved = true;
			// 确定直线终点
			m_Endp = point;
			// 复制兼容DC到客户区，以去掉之前的预览线段
			CDC* pDC = GetDC();
			CRect rect;
			GetClientRect(&rect);
			pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_pMemDC, 0, 0, SRCCOPY);
			// 画预览线
			if (currentTool == Tools.LINE)
				Tools.AntiAliasing(pDC, m_Startp.x, m_Startp.y, m_Endp.x, m_Endp.y, foreground, background);
			if (currentTool == Tools.RECTANGLE)
				Tools.Rectangle(pDC, m_Startp, m_Endp, foreground);
			if (currentTool == Tools.CIRCLE) {
				//pDC->SelectStockObject(NULL_BRUSH);
				//pDC->Ellipse(m_Startp.x, m_Startp.y, m_Endp.x, m_Endp.y);
				Tools.Circle(pDC, m_Startp, m_Endp, foreground);
			}
			// 释放资源
			ReleaseDC(pDC);
		}

		// ********************  画笔  ********************   
		if (currentTool == Tools.PENCIL && currentState == 1) {
			CDC* pDC = GetDC();
			m_Endp = point;
			Tools.AntiAliasing(pDC, m_Startp.x, m_Startp.y, m_Endp.x, m_Endp.y, foreground, foreground);
			ReleaseDC(pDC);
			m_Startp = m_Endp;
		}

		// ********************  橡皮  ********************   
		if (currentTool == Tools.ERASER && currentState == 1) {
			CDC* pDC = GetDC();
			m_Endp = point;
			CPen pen(PS_SOLID, eraserWidth, background);
			pDC->SelectObject(&pen);
			pDC->MoveTo(m_Startp);
			pDC->LineTo(m_Endp);
			ReleaseDC(pDC);
			m_Startp = m_Endp;
		}
	}

	// ********************  选择  ******************** 
	if (currentTool == Tools.CHOOSE && currentState == 1) {
		// 防止起点和终点相同出现问题
		moved = true;
		// 确定直线终点

		m_Endp = point;
		// 复制兼容DC到客户区，以去掉之前的预览线段
		CDC* pDC = GetDC();
		CRect rect;
		GetClientRect(&rect);
		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_pMemDC, 0, 0, SRCCOPY);
		chooseState = 1;
		// 画预览线
		CPen pen(PS_DASHDOT, 1, foreground);
		pDC->SelectObject(&pen);
		pDC->MoveTo(m_Startp.x, m_Startp.y);
		pDC->LineTo(m_Endp.x, m_Startp.y);
		pDC->LineTo(m_Endp.x, m_Endp.y);
		pDC->LineTo(m_Startp.x, m_Endp.y);
		pDC->LineTo(m_Startp.x, m_Startp.y);
		// 释放资源
		ReleaseDC(pDC);
	}
	CView::OnMouseMove(nFlags, point);
}


void CPaintView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (chooseState == 0) {
		// ********************  画线/矩形/圆  ********************
		if ((currentTool == Tools.LINE || currentTool == Tools.RECTANGLE || currentTool == Tools.CIRCLE) && currentState == 1) {
			if (moved) {
				// 画线并释放资源
				CDC* pDC = GetDC();
				CRect rect;
				GetClientRect(&rect);
				pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_pMemDC, 0, 0, SRCCOPY);
				if (currentTool == Tools.LINE)
					Tools.AntiAliasing(pDC, m_Startp.x, m_Startp.y, m_Endp.x, m_Endp.y, foreground, background);
				if (currentTool == Tools.RECTANGLE)
					Tools.Rectangle(pDC, m_Startp, m_Endp, foreground);
				if (currentTool == Tools.CIRCLE) {
					//pDC->SelectStockObject(NULL_BRUSH);
					//pDC->Ellipse(m_Startp.x, m_Startp.y, m_Endp.x, m_Endp.y);
					Tools.Circle(pDC, m_Startp, m_Endp, foreground);
				}
				moved = false;
				ReleaseDC(pDC);
			}
			currentState = 0;
			m_pMemBitmap.DeleteObject();
			m_pMemDC.DeleteDC();
		}

		// ********************  画笔  ********************
		if (currentTool == Tools.PENCIL && currentState == 1) {
			currentState = 0;
		}

		// ********************  橡皮  ********************
		if (currentTool == Tools.ERASER && currentState == 1) {
			currentState = 0;
		}

		// ********************  填充  ********************
		if (currentTool == Tools.FILL) {
			m_Endp = point;
			CDC* pDC = GetDC();
			CRect rect;
			GetClientRect(&rect);

			BITMAP bmpInfo;
			PBYTE pBits = GetPbits(&bmpInfo);
			Tools.Fill(pDC, m_Endp.x, m_Endp.y, pDC->GetPixel(m_Endp), foreground, rect.Height(), rect.Width(), pBits);
			// 建立新位图并载入位图信息到兼容DC
			CBitmap bitMap;
			bitMap.CreateBitmap(bmpInfo.bmWidth, bmpInfo.bmHeight, bmpInfo.bmPlanes, bmpInfo.bmBitsPixel, pBits);
			CDC m_MemDC;
			m_MemDC.CreateCompatibleDC(pDC);
			GetClientRect(&rect);
			m_MemDC.SelectObject(&bitMap);

			// 复制到客户区并释放资源
			pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_MemDC, 0, 0, SRCCOPY);
			bitMap.DeleteObject();
			m_MemDC.DeleteDC();
			ReleaseDC(pDC);
			delete pBits;
		}
	}

	// ********************  取色器  ********************
	if (currentTool == Tools.EYEDROPPER) {
		CDC* pDC = GetDC();
		foreground = pDC->GetPixel(point);
		ReleaseDC(pDC);
	}

	// ********************  选择  ******************** 
	if (currentTool == Tools.CHOOSE && currentState == 1) {
		if (moved) {
			// 画线并释放资源
			CDC* pDC = GetDC();
			CRect rect;
			GetClientRect(&rect);
			pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_pMemDC, 0, 0, SRCCOPY);

			//m_cMemDC.BitBlt(0, 0, rect.Width(), rect.Height(), &m_sMemDC, 0, 0, DSTINVERT);
			//m_cMemDC.BitBlt(m_Startp.x, m_Startp.y, m_Endp.x - m_Startp.x, m_Endp.y - m_Startp.y, &m_sMemDC, m_Startp.x, m_Startp.y, SRCCOPY);

			P[0] = CPoint(m_Startp.x, m_Startp.y);
			P[1] = CPoint(m_Endp.x, m_Startp.y);
			P[2] = CPoint(m_Endp.x, m_Endp.y);
			P[3] = CPoint(m_Startp.x, m_Endp.y);

			Draw(pDC);

			moved = false;
			ReleaseDC(pDC);
		}
		currentState = 0;
		m_pMemBitmap.DeleteObject();
		m_pMemDC.DeleteDC();
	}
	CView::OnLButtonUp(nFlags, point);
}

BOOL CPaintView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN && chooseState == 1)//回车键
		{
			CDC* pDC = GetDC();
			CRect rect;
			GetClientRect(&rect);
			//pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_cMemDC, 0, 0, SRCCOPY);
			m_sMemBitmap.DeleteObject();
			m_sMemDC.DeleteDC();
			m_cMemBitmap.DeleteObject();
			m_cMemDC.DeleteDC();
			chooseState = 0;


			return TRUE;
		}
	}
	return CView::PreTranslateMessage(pMsg);
}

void CPaintView::OnToolLine()
{
	// TODO: 在此添加命令处理程序代码
	currentTool = Tools.LINE;
	currentState = 0;
	moved = false;
}

void CPaintView::OnToolPencil()
{
	// TODO: 在此添加命令处理程序代码
	currentTool = Tools.PENCIL;
	currentState = 0;
}

void CPaintView::OnToolCircle()
{
	// TODO: 在此添加命令处理程序代码
	currentTool = Tools.CIRCLE;
	currentState = 0;
	moved = false;
}

void CPaintView::OnToolRectangle()
{
	// TODO: 在此添加命令处理程序代码
	currentTool = Tools.RECTANGLE;
	currentState = 0;
	moved = false;
}

void CPaintView::OnToolFill()
{
	// TODO: 在此添加命令处理程序代码
	currentTool = Tools.FILL;
}

void CPaintView::OnToolEyedropper()
{
	// TODO: 在此添加命令处理程序代码
	currentTool = Tools.EYEDROPPER;
}

void CPaintView::OnToolEraser()
{
	// TODO: 在此添加命令处理程序代码
	currentTool = Tools.ERASER;
	currentState = 0;
}

void CPaintView::OnToolForeground()
{
	// TODO: 在此添加命令处理程序代码
	CColorDialog dlg;
	dlg.DoModal();
	foreground = dlg.GetColor();

}

void CPaintView::OnToolBackground()
{
	// TODO: 在此添加命令处理程序代码
	CColorDialog dlg;
	dlg.DoModal();
	background = dlg.GetColor();
}

void CPaintView::OnToolNone()
{
	// TODO: 在此添加命令处理程序代码
	currentTool = Tools.NONE;
}

void CPaintView::OnToolChoose()
{
	// TODO: 在此添加命令处理程序代码
	currentTool = Tools.CHOOSE;
	currentState = 0;
	moved = false;
}

void CPaintView::OnToolDown()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_sMemDC, 0, 0, SRCCOPY);
	Identity();
	A[2][0] = 0;
	A[2][1] = 10;
	Trans();
	Draw(pDC);
}


void CPaintView::OnToolLeft()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_sMemDC, 0, 0, SRCCOPY);
	Identity();
	A[2][0] = -10;
	A[2][1] = 0;
	Trans();
	Draw(pDC);
}


void CPaintView::OnToolUp()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_sMemDC, 0, 0, SRCCOPY);
	Identity();
	A[2][0] = 0;
	A[2][1] = -10;
	Trans();
	Draw(pDC);
}


void CPaintView::OnToolRight()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_sMemDC, 0, 0, SRCCOPY);
	Identity();
	A[2][0] = 10;
	A[2][1] = 0;
	Trans();
	Draw(pDC);
}

void CPaintView::OnToolContrarotate()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_sMemDC, 0, 0, SRCCOPY);

	pDC->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
	pDC->SetWindowExt(rect.Width(), rect.Height());//设置窗口范围
	pDC->SetViewportExt(rect.Width(), -rect.Height());//设置视区范围,x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg((P[0].x + P[2].x) / 2, (P[1].y + P[3].y) / 2);//客户区中心为原点

	int dx = (P[0].x + P[2].x) / 2;
	int dy = (P[1].y + P[3].y) / 2;

	P[0].x -= dx;
	P[0].y -= dy;
	P[1].x -= dx;
	P[1].y -= dy;
	P[2].x -= dx;
	P[2].y -= dy;
	P[3].x -= dx;
	P[3].y -= dy;

	Identity();
	A[0][0] = cos(M_PI * -15 / 180);
	A[0][1] = sin(M_PI * -15 / 180);
	A[1][0] = -sin(M_PI * -15 / 180);
	A[1][1] = cos(M_PI * -15 / 180);

	Trans();
	Draw(pDC);

	P[0].x += dx;
	P[0].y += dy;
	P[1].x += dx;
	P[1].y += dy;
	P[2].x += dx;
	P[2].y += dy;
	P[3].x += dx;
	P[3].y += dy;
}


void CPaintView::OnToolRotate()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_sMemDC, 0, 0, SRCCOPY);

	pDC->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
	pDC->SetWindowExt(rect.Width(), rect.Height());//设置窗口范围
	pDC->SetViewportExt(rect.Width(), -rect.Height());//设置视区范围,x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg((P[0].x + P[2].x) / 2, (P[1].y + P[3].y) / 2);//客户区中心为原点

	int dx = (P[0].x + P[2].x) / 2;
	int dy = (P[1].y + P[3].y) / 2;

	P[0].x -= dx;
	P[0].y -= dy;
	P[1].x -= dx;
	P[1].y -= dy;
	P[2].x -= dx;
	P[2].y -= dy;
	P[3].x -= dx;
	P[3].y -= dy;

	Identity();
	A[0][0] = cos(M_PI * 15 / 180);
	A[0][1] = sin(M_PI * 15 / 180);
	A[1][0] = -sin(M_PI * 15 / 180);
	A[1][1] = cos(M_PI * 15 / 180);

	Trans();
	Draw(pDC);

	P[0].x += dx;
	P[0].y += dy;
	P[1].x += dx;
	P[1].y += dy;
	P[2].x += dx;
	P[2].y += dy;
	P[3].x += dx;
	P[3].y += dy;
}

void CPaintView::OnToolXshear()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_sMemDC, 0, 0, SRCCOPY);

	pDC->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
	pDC->SetWindowExt(rect.Width(), rect.Height());//设置窗口范围
	pDC->SetViewportExt(rect.Width(), -rect.Height());//设置视区范围,x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg((P[0].x + P[2].x) / 2, (P[1].y + P[3].y) / 2);//客户区中心为原点

	int dx = (P[0].x + P[2].x) / 2;
	int dy = (P[1].y + P[3].y) / 2;

	P[0].x -= dx;
	P[0].y -= dy;
	P[1].x -= dx;
	P[1].y -= dy;
	P[2].x -= dx;
	P[2].y -= dy;
	P[3].x -= dx;
	P[3].y -= dy;

	Identity();
	A[1][0] = 0.1;

	Trans();
	Draw(pDC);

	P[0].x += dx;
	P[0].y += dy;
	P[1].x += dx;
	P[1].y += dy;
	P[2].x += dx;
	P[2].y += dy;
	P[3].x += dx;
	P[3].y += dy;
}


void CPaintView::OnToolYshear()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_sMemDC, 0, 0, SRCCOPY);

	pDC->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
	pDC->SetWindowExt(rect.Width(), rect.Height());//设置窗口范围
	pDC->SetViewportExt(rect.Width(), -rect.Height());//设置视区范围,x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg((P[0].x + P[2].x) / 2, (P[1].y + P[3].y) / 2);//客户区中心为原点

	int dx = (P[0].x + P[2].x) / 2;
	int dy = (P[1].y + P[3].y) / 2;

	P[0].x -= dx;
	P[0].y -= dy;
	P[1].x -= dx;
	P[1].y -= dy;
	P[2].x -= dx;
	P[2].y -= dy;
	P[3].x -= dx;
	P[3].y -= dy;

	Identity();
	A[0][1] = 0.1;

	Trans();
	Draw(pDC);

	P[0].x += dx;
	P[0].y += dy;
	P[1].x += dx;
	P[1].y += dy;
	P[2].x += dx;
	P[2].y += dy;
	P[3].x += dx;
	P[3].y += dy;
}

void CPaintView::OnToolZoomin()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_sMemDC, 0, 0, SRCCOPY);

	pDC->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
	pDC->SetWindowExt(rect.Width(), rect.Height());//设置窗口范围
	pDC->SetViewportExt(rect.Width(), -rect.Height());//设置视区范围,x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg((P[0].x + P[1].x) / 2, (P[1].y + P[2].y) / 2);//客户区中心为原点

	int dx = (P[0].x + P[1].x) / 2;
	int dy = (P[1].y + P[2].y) / 2;

	P[0].x -= dx;
	P[0].y -= dy;
	P[1].x -= dx;
	P[1].y -= dy;
	P[2].x -= dx;
	P[2].y -= dy;
	P[3].x -= dx;
	P[3].y -= dy;

	Identity();
	A[0][0] = 1.1;
	A[1][1] = 1.1;
	Trans();
	Draw(pDC);

	P[0].x += dx;
	P[0].y += dy;
	P[1].x += dx;
	P[1].y += dy;
	P[2].x += dx;
	P[2].y += dy;
	P[3].x += dx;
	P[3].y += dy;
}


void CPaintView::OnToolZoomout()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_sMemDC, 0, 0, SRCCOPY);

	pDC->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
	pDC->SetWindowExt(rect.Width(), rect.Height());//设置窗口范围
	pDC->SetViewportExt(rect.Width(), -rect.Height());//设置视区范围,x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg((P[0].x + P[1].x) / 2, (P[1].y + P[2].y) / 2);//客户区中心为原点

	int dx = (P[0].x + P[1].x) / 2;
	int dy = (P[1].y + P[2].y) / 2;

	P[0].x -= dx;
	P[0].y -= dy;
	P[1].x -= dx;
	P[1].y -= dy;
	P[2].x -= dx;
	P[2].y -= dy;
	P[3].x -= dx;
	P[3].y -= dy;

	Identity();
	A[0][0] = 0.9;
	A[1][1] = 0.9;
	Trans();
	Draw(pDC);

	P[0].x += dx;
	P[0].y += dy;
	P[1].x += dx;
	P[1].y += dy;
	P[2].x += dx;
	P[2].y += dy;
	P[3].x += dx;
	P[3].y += dy;

	ReleaseDC(pDC);
}


PBYTE CPaintView::GetPbits(BITMAP* bmpInfo) {
	CDC* pDC = GetDC();
	// 获取客户区大小
	CRect rect;
	GetClientRect(&rect);
	CDC m_MemDC;
	CBitmap m_MemBitmap;
	// 创建兼容DC
	m_MemDC.CreateCompatibleDC(pDC);
	// 为兼容DC创建位图
	m_MemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	m_MemDC.SelectObject(&m_MemBitmap);
	// 复制客户区DC到兼容DC
	m_MemDC.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
	m_MemBitmap.GetBitmap(bmpInfo);
	long sizeBits = bmpInfo->bmWidth * bmpInfo->bmHeight * (bmpInfo->bmWidthBytes / bmpInfo->bmWidth);
	PBYTE pBits = new BYTE[sizeBits];
	ZeroMemory(pBits, sizeBits);
	m_MemBitmap.GetBitmapBits(sizeBits, pBits);
	return pBits;
}

void CPaintView::SaveHistory() {
	CDC* pDC = GetDC();
	// 获取客户区大小
	CRect rect;
	GetClientRect(&rect);
	CDC m_hMemDC;
	CBitmap m_hMemBitmap;
	// 创建兼容DC
	m_hMemDC.CreateCompatibleDC(pDC);
	// 为兼容DC创建位图
	m_hMemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	m_hMemDC.SelectObject(&m_hMemBitmap);
	// 复制客户区DC到兼容DC
	m_hMemDC.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);

	// 保存历史记录
	SaveHistory(&m_hMemBitmap);
	// 释放资源
	ReleaseDC(pDC);
}

void CPaintView::SaveHistory(CBitmap* m_MemBitmap) {
	// 获取当前位图信息并保存入栈
	BITMAP bmpInfo;
	m_MemBitmap->GetBitmap(&bmpInfo);
	long sizeBits = bmpInfo.bmWidth * bmpInfo.bmHeight * (bmpInfo.bmWidthBytes / bmpInfo.bmWidth);
	PBYTE pBits = new BYTE[sizeBits];
	ZeroMemory(pBits, sizeBits);
	m_MemBitmap->GetBitmapBits(sizeBits, pBits);
	history.push(BitMapInfo(bmpInfo, sizeBits, pBits));
}

void CPaintView::OnRevoke()
{
	// TODO: 在此添加命令处理程序代码
	if (!history.empty()) {
		// 读取栈中位图信息
		BitMapInfo bitMapInfo = history.top();

		BITMAP bmpInfo = bitMapInfo.bitmap;
		long sizeBits = bitMapInfo.sizeBits;
		PBYTE pBits = bitMapInfo.pBits;
		// 建立新位图并载入位图信息到兼容DC
		CBitmap hBitMap;
		hBitMap.CreateBitmap(bmpInfo.bmWidth, bmpInfo.bmHeight, bmpInfo.bmPlanes, bmpInfo.bmBitsPixel, pBits);
		CDC* pDC = GetDC();
		CDC m_hMemDC;
		m_hMemDC.CreateCompatibleDC(pDC);
		CRect rect;
		GetClientRect(&rect);
		m_hMemDC.SelectObject(&hBitMap);
		
		// 复制到客户区并释放资源
		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_hMemDC, 0, 0, SRCCOPY);
		hBitMap.DeleteObject();
		m_hMemDC.DeleteDC();
		ReleaseDC(pDC);
		history.pop();
		delete pBits;
	}
}

void CPaintView::Identity()
{
	A[0][0] = 1.0; A[0][1] = 0.0; A[0][2] = 0.0;
	A[1][0] = 0.0; A[1][1] = 1.0; A[1][2] = 0.0;
	A[2][0] = 0.0; A[2][1] = 0.0; A[2][2] = 1.0;
}

void CPaintView::Trans()
{
	double X[4], Y[4], C[4];
	for (int i = 0; i < 4; i++)
	{
		X[i] = P[i].x * A[0][0] + P[i].y * A[1][0] + A[2][0];
		Y[i] = P[i].x * A[0][1] + P[i].y * A[1][1] + A[2][1];
		C[i] = P[i].x * A[0][2] + P[i].y * A[1][2] + A[2][2];
		P[i].x = X[i] / C[i];
		P[i].y = Y[i] / C[i];
	}
}

void CPaintView::Draw(CDC* pDC) {
	CPen pen(PS_DASHDOT, 1, foreground);
	pDC->SelectObject(&pen);
	pDC->MoveTo(P[0].x, P[0].y);
	pDC->LineTo(P[1].x, P[1].y);
	pDC->LineTo(P[2].x, P[2].y);
	pDC->LineTo(P[3].x, P[3].y);
	pDC->LineTo(P[0].x, P[0].y);
}