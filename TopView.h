#pragma once

#include "AUVDoc.h"

// CTopView ��ͼ

class CTopView : public CView
{
	DECLARE_DYNCREATE(CTopView)

protected:
	CTopView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CTopView();

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

	CPoint pointTime;
	CPoint pointMillisecond;
	CPoint pointDate;
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


