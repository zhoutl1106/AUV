#pragma once


// CLocationView 视图

class CLocationView : public CView
{
	DECLARE_DYNCREATE(CLocationView)

protected:
	CLocationView();           // 动态创建所使用的受保护的构造函数
	virtual ~CLocationView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
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


