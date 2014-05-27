#pragma once

#include "AUVDoc.h"

// CBottomView 视图

class CBottomView : public CView
{
	DECLARE_DYNCREATE(CBottomView)

protected:
	CBottomView();           // 动态创建所使用的受保护的构造函数
	virtual ~CBottomView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	CAUVDoc* GetDocument() const;

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


