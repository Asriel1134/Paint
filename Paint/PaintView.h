
// PaintView.h: CPaintView 类的接口
//

#pragma once
#include "TOOLS.h"
#include "BitMapInfo.h"
#include <stack>


class CPaintView : public CView
{
protected: // 仅从序列化创建
	CPaintView() noexcept;
	DECLARE_DYNCREATE(CPaintView)

// 特性
public:
	CPaintDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CPaintView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
private:
	std::stack<BitMapInfo> history;
	int currentState;
	bool moved;
	long foreground;
	long background;
	int currentTool;
	double A[3][3];
	CPoint m_Startp;
	CPoint m_Endp;
	TOOLS Tools;
	CDC m_pMemDC;
	CBitmap m_pMemBitmap;
	// 选择的区域
	CDC m_cMemDC;
	CBitmap m_cMemBitmap;
	// 保存选择前的位图
	CDC m_sMemDC;
	CBitmap m_sMemBitmap;

public:
	afx_msg void OnToolForeground();
	afx_msg void OnToolBackground();
	PBYTE GetPbits(BITMAP* bmpInfo);
	void SaveHistory();
	void SaveHistory(CBitmap* m_pMemBitmap);
	void CPaintView::Trans();
	afx_msg void OnToolLine();
	afx_msg void OnRevoke();
	void Identity();
	afx_msg void OnToolPencil();
	afx_msg void OnToolCircle();
	afx_msg void OnToolRectangle();
	afx_msg void OnToolFill();
	afx_msg void OnToolEyedropper();
	afx_msg void OnToolEraser();
private:
	int eraserWidth;
public:
	afx_msg void OnToolNone();
	afx_msg void OnToolChoose();
	afx_msg void OnToolZoomin();
	afx_msg void OnToolZoomout();
private:
	int chooseState;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#ifndef _DEBUG  // PaintView.cpp 中的调试版本
inline CPaintDoc* CPaintView::GetDocument() const
   { return reinterpret_cast<CPaintDoc*>(m_pDocument); }
#endif

