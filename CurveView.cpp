// CurveView.cpp : implementation file
//

//	Name: CurveView.cpp
//	Function: class CCurveView
//	Date: Feb.8, 2011
//	Author: Tianli Zhou

#include "stdafx.h"
#include "AUV.h"
#include "AUVView.h"
#include "MainFrm.h"
#include "AUVDoc.h"
#include "OpenGLView.h"
#include "CurveView.h"
#include "LocationView.h"
#include "DriveView.h"
#include "ShowView.h"
#include "CameraView.h"

// CCurveView

IMPLEMENT_DYNCREATE(CCurveView, CAUVView)

CCurveView::CCurveView()
{

}

CCurveView::~CCurveView()
{
}

BEGIN_MESSAGE_MAP(CCurveView, CAUVView)
	ON_WM_CREATE()
//	ON_WM_CONTEXTMENU()
ON_WM_DESTROY()
//ON_WM_PAINT()
//ON_WM_TIMER()
ON_WM_TIMER()
ON_WM_LBUTTONDBLCLK()
ON_WM_ERASEBKGND()
ON_WM_SIZE()
ON_WM_LBUTTONDOWN()
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONUP()
ON_WM_CHAR()
END_MESSAGE_MAP()


// CCurveView drawing

void CCurveView::OnDraw(CDC* pDC)
{
// TODO: add draw code here
	CRect rectClient;
	GetClientRect(rectClient);
		
	CAUVDoc* pDoc = GetDocument();
		
	CDC MemDC; //首先定义一个显示设备对象
	CBitmap MemBitmap;//定义一个位图对象
	//随后建立与屏幕显示兼容的内存显示设备
	MemDC.CreateCompatibleDC(NULL);
	//这时还不能绘图，因为没有地方画 ^_^
	//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小
	MemBitmap.CreateCompatibleBitmap(pDC,rectClient.Width(),rectClient.Height());
	//将位图选入到内存显示设备中
	//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
	//先用背景色将位图清除干净，这里我用的是白色作为背景
	//你也可以用自己应该用的颜色
	MemDC.FillSolidRect(0,0,rectClient.right,rectClient.bottom,RGB(50,79,163));
	//绘图
	CPen redPen(PS_SOLID,4,RGB(255,0,0));
	CPen blackPen(PS_SOLID,1,RGB(0,0,0));
	CPen yellowPen(PS_SOLID,2,RGB(242,249,0));
	CPen greenPen(PS_SOLID,2,RGB(0,255,0));
	//draw axis
	CFont newFont;
	newFont.CreateFont(80*rectClient.right/1440,40*rectClient.right/1440,0,0,100,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH|FF_MODERN,NULL);
	CFont newFont2;
	newFont2.CreateFont(60*rectClient.right/1440,30*rectClient.right/1440,0,0,80,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH|FF_MODERN,NULL);
	CFont *oldFont;
	oldFont = MemDC.SelectObject(&newFont);
	MemDC.SetTextColor(RGB(196,211,236));
	MemDC.TextOut(rectClient.right/2-350,rectClient.bottom/9-75,"深度-时间曲线",13);
	MemDC.SelectObject(oldFont);
	MemDC.MoveTo(rectClient.right/7,rectClient.bottom/7);
	MemDC.LineTo(6*rectClient.right/7,rectClient.bottom/7);//-
		
	MemDC.MoveTo(rectClient.right/7,rectClient.bottom/7);
	MemDC.LineTo(rectClient.right/7,6*rectClient.bottom/7);//|
		
	MemDC.MoveTo(rectClient.right/7,6*rectClient.bottom/7);
	MemDC.LineTo(rectClient.right/7-rectClient.right/80,6*rectClient.bottom/7-rectClient.bottom/50);
		
	MemDC.MoveTo(rectClient.right/7,6*rectClient.bottom/7);
	MemDC.LineTo(rectClient.right/7+rectClient.right/80,6*rectClient.bottom/7-rectClient.bottom/50);//|^
		
	MemDC.MoveTo(6*rectClient.right/7,rectClient.bottom/7);
	MemDC.LineTo(6*rectClient.right/7-rectClient.right/50,rectClient.bottom/7-rectClient.bottom/80);
		
	MemDC.MoveTo(6*rectClient.right/7,rectClient.bottom/7);
	MemDC.LineTo(6*rectClient.right/7-rectClient.right/50,rectClient.bottom/7+rectClient.bottom/80);//-^
		
	CString string2=_T("深度");//y
	MemDC.SetTextColor(RGB(250,0,0));
	MemDC.TextOut(rectClient.right/7-rectClient.right/40,6*rectClient.bottom/7+rectClient.bottom/40,string2);
	CString string3=_T("时间");//x
	MemDC.SetTextColor(RGB(250,0,0));
	MemDC.TextOut(6*rectClient.right/7-rectClient.right/50,rectClient.bottom/7+rectClient.bottom/80,string3);
		
	//draw Curve;
	MemDC.SelectObject(yellowPen);
	int j = 0;
	while(j<798)
	{
		MemDC.MoveTo(rectClient.right/7+(j)*rectClient.right/1440,rectClient.bottom/7-(pDoc->depth[j/2])*rectClient.right/144);
		MemDC.LineTo(rectClient.right/7+(j+1)*rectClient.right/1440,rectClient.bottom/7-(pDoc->depth[j/2+1])*rectClient.right/144);
		j+=2;
	}
		
	//draw demarcation strip
	MemDC.SelectObject(redPen);
	MemDC.MoveTo(rectClient.right/7+2*pDoc->count*rectClient.right/1440,6*rectClient.bottom/7+rectClient.bottom/80*rectClient.right/1440);
	MemDC.LineTo(rectClient.right/7+2*pDoc->count*rectClient.right/1440,rectClient.bottom/7-rectClient.bottom/80*rectClient.right/1440);
	MemDC.SelectObject(greenPen);
	MemDC.MoveTo(clickpoint.x,5*rectClient.bottom/6+rectClient.bottom/80);
	MemDC.LineTo(clickpoint.x,rectClient.bottom/6-rectClient.bottom/80);
	MemDC.SelectObject(newFont2);
	MemDC.SetTextColor(RGB(0,255,0));
	char ch[50];
	if((clickpoint.x-rectClient.right/7)>=0 && (clickpoint.x-rectClient.right/7)*720/(rectClient.right+1) < MAX_COUNT_POINTS-1)
		StringCchPrintf(ch,50,"点击处深度为 %2.2f m",pDoc->depth[(clickpoint.x-rectClient.right/7)*720/(rectClient.right+1)]);
	else
		StringCchPrintf(ch,50,"请选择曲线内的位置",0);
	MemDC.TextOut(rectClient.right/2-280,7*rectClient.bottom/8,ch);
	MemDC.SelectObject(blackPen);
		
	//将内存中的图拷贝到屏幕上进行显示
	pDC->BitBlt(0,0,rectClient.right,rectClient.bottom,&MemDC,0,0,SRCCOPY);
	//绘图完成后的清理
	redPen.DeleteObject();
	blackPen.DeleteObject();
	yellowPen.DeleteObject();
	greenPen.DeleteObject();
	newFont.DeleteObject();
	newFont2.DeleteObject();
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();	
}


// CCurveView diagnostics

#ifdef _DEBUG
void CCurveView::AssertValid() const
{
	CAUVView::AssertValid();
}

#ifndef _WIN32_WCE
void CCurveView::Dump(CDumpContext& dc) const
{
	CAUVView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCurveView message handlers


int CCurveView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
// 	SetTimer(2,200,NULL);
	// TODO:  Add your specialized creation code here
	return 0;
}

CAUVDoc* CCurveView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAUVDoc)));
	return (CAUVDoc*)m_pDocument;
}


//void CCurveView::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
//{
//	// TODO: Add your message handler code here
//}


void CCurveView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
}


//void CCurveView::OnPaint()
//{
//	
//	OnDraw(&dc);
//}


void CCurveView::OnInitialUpdate()
{
	CAUVView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	m_pMainWnd =  (CMainFrame *) AfxGetApp()->m_pMainWnd;
}


void CCurveView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
// 	Invalidate(FALSE);
	CAUVView::OnTimer(nIDEvent);
}


void CCurveView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect,rectClient;
	GetParentFrame()->GetClientRect(&rect);
	GetClientRect(&rectClient);
	CMainFrame* pMain = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	if(rectClient.right < rect.right/2)
	{     
		pMain->m_wndSplitter.SetRowInfo(0,rect.bottom,0);
		pMain->m_wndSplitter.SetColumnInfo(0,0,0);
		pMain->m_wndSplitter.SetColumnInfo(1,0,0);
		pMain->m_wndSplitter.SetColumnInfo(2,rect.right,0);
		pMain->m_wndSplitter.RecalcLayout();
	}
	else
	{
		pMain->m_wndSplitter.SetRowInfo(0,rect.bottom/5,0);
		pMain->m_wndSplitter.SetRowInfo(1,rect.bottom*3/5,0);
		pMain->m_wndSplitter.SetColumnInfo(0,rect.right/7,0);
		pMain->m_wndSplitter.SetColumnInfo(1,rect.right*5/7,0);
		pMain->m_wndSplitter.RecalcLayout();
	}
	Invalidate(FALSE);
	//CAUVView::OnLButtonDblClk(nFlags, point);
}


BOOL CCurveView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CAUVView::OnEraseBkgnd(pDC);
}


void CCurveView::OnSize(UINT nType, int cx, int cy)
{
	CAUVView::OnSize(nType, cx, cy);
	Invalidate(FALSE);
	// TODO: 在此处添加消息处理程序代码
}


void CCurveView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rectClient;
	GetClientRect(rectClient);
	if(point.x>rectClient.right/7 &&
		point.x<6*rectClient.right/7 &&
		point.y>rectClient.bottom/7 &&
		point.y<6*rectClient.bottom/7)
		clickpoint = point;
	CAUVView::OnLButtonDown(nFlags, point);
}


void CCurveView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CAUVView::OnRButtonDown(nFlags, point);
}


void CCurveView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CAUVView::OnRButtonUp(nFlags, point);
}

void CCurveView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CAUVDoc* pDoc = GetDocument();
	CRect rect,rectClient;
	GetParentFrame()->GetClientRect(&rect);
	GetClientRect(&rectClient);
	CMainFrame* pMain = (CMainFrame* )AfxGetApp()->m_pMainWnd;

	switch(nChar)
	{
	case '1':
		if(rectClient.Width()>rect.Width()/2)
		{
			pMain->m_wndSplitter.SetRowInfo(0,rect.bottom,0);
			pMain->m_wndSplitter.SetColumnInfo(0,rect.right,0);
			pMain->m_wndSplitter.RecalcLayout();
			pMain->SetActiveView((COpenGLView*)pMain->m_wndSplitter.GetPane(0,0));
		}
		break;
	case '2':
		if(rectClient.Width()>rect.Width()/2)
		{
			pMain->m_wndSplitter.SetRowInfo(0,rect.bottom,0);
			pMain->m_wndSplitter.SetColumnInfo(0,0,0);
			pMain->m_wndSplitter.SetColumnInfo(1,0,0);
			pMain->m_wndSplitter.SetColumnInfo(2,rect.right,0);
			pMain->m_wndSplitter.RecalcLayout();
			pMain->SetActiveView((CCurveView*)pMain->m_wndSplitter.GetPane(0,3));
		}
		break;
	case '3':
		if(rectClient.Width()>rect.Width()/2)
		{
			pMain->m_wndSplitter.SetRowInfo(0,0,0);
			pMain->m_wndSplitter.SetRowInfo(1,0,0);
			pMain->m_wndSplitter.SetRowInfo(2,rect.bottom,0);
			pMain->m_wndSplitter.SetColumnInfo(0,rect.right,0);
			pMain->m_wndSplitter.RecalcLayout();
			pMain->SetActiveView((CLocationView*)pMain->m_wndSplitter.GetPane(3,0));
		}
		break;
	case '4':
		if(rectClient.Width()>rect.Width()/2)
		{
			pMain->m_wndSplitter.SetRowInfo(0,0,0);
			pMain->m_wndSplitter.SetRowInfo(1,0,0);
			pMain->m_wndSplitter.SetRowInfo(2,rect.bottom,0);
			pMain->m_wndSplitter.SetColumnInfo(0,0,0);
			pMain->m_wndSplitter.SetColumnInfo(1,0,0);
			pMain->m_wndSplitter.SetColumnInfo(2,rect.right,0);
			pMain->m_wndSplitter.RecalcLayout();
			pMain->SetActiveView((CCameraView*)pMain->m_wndSplitter.GetPane(3,3));
		}
		break;
	}
	CView::OnChar(nChar, nRepCnt, nFlags);
}
