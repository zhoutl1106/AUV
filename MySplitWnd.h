#pragma once

// CMySplitWnd

class CMySplitWnd : public CSplitterWnd
{
	DECLARE_DYNAMIC(CMySplitWnd)

public:
	CMySplitWnd();
	virtual ~CMySplitWnd();
	BOOL ReplaceView(int row, int col, CRuntimeClass *pViewClass, SIZE size);

protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
//	BOOL IsMax;
public:
//	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
};


