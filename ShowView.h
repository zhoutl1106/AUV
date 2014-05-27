#pragma once

#include "AUVDoc.h"
// CShowView view

class CShowView : public CView
{
	DECLARE_DYNCREATE(CShowView)

protected:
	CShowView();           // protected constructor used by dynamic creation
	virtual ~CShowView();

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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


