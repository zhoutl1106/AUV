#pragma once


// CLocationView ��ͼ

class CLocationView : public CView
{
	DECLARE_DYNCREATE(CLocationView)

protected:
	CLocationView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CLocationView();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	CAUVDoc* GetDocument() const;
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


