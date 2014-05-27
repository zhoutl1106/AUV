#pragma once

#include "AUVDoc.h"
// CCameraView view

class CCameraView : public CView
{
	DECLARE_DYNCREATE(CCameraView)

protected:
	CCameraView();           // protected constructor used by dynamic creation
	virtual ~CCameraView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
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
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	BOOL mouseleftdown;
	BOOL zoom;
	CPoint mousestartpoint;
	CPoint mouseendpoint;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL DrawZoom;
	float xlocation[MAX_COUNT_POINTS];
	float ylocation[MAX_COUNT_POINTS];
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL m_model;
};


