// MySplitWnd.cpp : implementation file
//

#include "stdafx.h"
#include "AUV.h"
#include "MySplitWnd.h"
#include "AUVDoc.h"

// CMySplitWnd

IMPLEMENT_DYNAMIC(CMySplitWnd, CSplitterWnd)

CMySplitWnd::CMySplitWnd()
{

}

CMySplitWnd::~CMySplitWnd()
{
}


BEGIN_MESSAGE_MAP(CMySplitWnd, CSplitterWnd)
//	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CMySplitWnd message handlers

void CMySplitWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CSplitterWnd::OnLButtonDown(nFlags, point);
}


void CMySplitWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CSplitterWnd::OnMouseMove(nFlags, point);
}
BOOL CMySplitWnd::ReplaceView(int row, int col, CRuntimeClass *pViewClass, SIZE size)
{
	CCreateContext context;
	BOOL bSetActive;


	if ((GetPane(row,col)->IsKindOf(pViewClass))==TRUE)
		return FALSE;


	// Get pointer to CDocument object so that it can be used in the creation 
	// process of the new view
	CDocument* pDoc= ((CView *)GetPane(row,col))->GetDocument();
	CView * pActiveView=GetParentFrame()->GetActiveView();
	if (pActiveView==NULL || pActiveView==GetPane(row,col))
		bSetActive=TRUE;
	else
		bSetActive=FALSE;
	// set flag so that document will not be deleted when view is destroyed
	// Delete existing view 
	((CView *) GetPane(row,col))->DestroyWindow();
	// set flag back to default 
	// Create new view                      

	context.m_pNewViewClass=pViewClass;
	context.m_pCurrentDoc=pDoc;
	context.m_pNewDocTemplate=NULL;
	context.m_pLastView=NULL;
	context.m_pCurrentFrame=NULL;

	CreateView(row,col,pViewClass,size, &context);

	CView * pNewView= (CView *)GetPane(row,col);

	if (bSetActive==TRUE)
		GetParentFrame()->SetActiveView(pNewView);

	RecalcLayout(); 
	GetPane(row,col)->SendMessage(WM_PAINT);

	return TRUE;
}

