
// AUVView.h : CAUVView 类的接口
//

#pragma once

#include "AUVDoc.h"
#include "Joystick.h"

class CAUVView : public CView
{
protected: // 仅从序列化创建
	CAUVView();
	DECLARE_DYNCREATE(CAUVView)

// 特性
public:
	CAUVDoc* GetDocument() const;

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
	virtual ~CAUVView();
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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LONG OnCommunication(WPARAM ch, LPARAM port);
	void ProcessJoystick(void);
	bool m_show;
	unsigned int m_sentCount;
	unsigned int m_receiveCount;

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // AUVView.cpp 中的调试版本
inline CAUVDoc* CAUVView::GetDocument() const
   { return reinterpret_cast<CAUVDoc*>(m_pDocument); }
#endif

