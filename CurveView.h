#pragma once

#include"MainFrm.h"
#include"AUVView.h"
// CCurveView view

//	Name: Curve.h
//	Function: class CCurveView
//	Date: Feb.8, 2011
//	Author: Tianli Zhou

class CCurveView : public CAUVView
{
	DECLARE_DYNCREATE(CCurveView)

protected:
	CCurveView();           // protected constructor used by dynamic creation
	virtual ~CCurveView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	CMainFrame* m_pMainWnd;
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
//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnDestroy();
//	afx_msg void OnPaint();
	virtual void OnInitialUpdate();
	CAUVDoc* GetDocument() const;
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CPoint clickpoint;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


