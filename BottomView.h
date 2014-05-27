#pragma once

#include "AUVDoc.h"

// CBottomView ��ͼ

class CBottomView : public CView
{
	DECLARE_DYNCREATE(CBottomView)

protected:
	CBottomView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CBottomView();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
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


