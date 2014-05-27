
// AUVView.h : CAUVView ��Ľӿ�
//

#pragma once

#include "AUVDoc.h"
#include "Joystick.h"

class CAUVView : public CView
{
protected: // �������л�����
	CAUVView();
	DECLARE_DYNCREATE(CAUVView)

// ����
public:
	CAUVDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CAUVView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // AUVView.cpp �еĵ��԰汾
inline CAUVDoc* CAUVView::GetDocument() const
   { return reinterpret_cast<CAUVDoc*>(m_pDocument); }
#endif

