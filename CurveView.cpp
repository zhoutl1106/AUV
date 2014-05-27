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
		
	CDC MemDC; //���ȶ���һ����ʾ�豸����
	CBitmap MemBitmap;//����һ��λͼ����
	//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	MemDC.CreateCompatibleDC(NULL);
	//��ʱ�����ܻ�ͼ����Ϊû�еط��� ^_^
	//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
	MemBitmap.CreateCompatibleBitmap(pDC,rectClient.Width(),rectClient.Height());
	//��λͼѡ�뵽�ڴ���ʾ�豸��
	//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
	//���ñ���ɫ��λͼ����ɾ����������õ��ǰ�ɫ��Ϊ����
	//��Ҳ�������Լ�Ӧ���õ���ɫ
	MemDC.FillSolidRect(0,0,rectClient.right,rectClient.bottom,RGB(50,79,163));
	//��ͼ
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
	MemDC.TextOut(rectClient.right/2-350,rectClient.bottom/9-75,"���-ʱ������",13);
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
		
	CString string2=_T("���");//y
	MemDC.SetTextColor(RGB(250,0,0));
	MemDC.TextOut(rectClient.right/7-rectClient.right/40,6*rectClient.bottom/7+rectClient.bottom/40,string2);
	CString string3=_T("ʱ��");//x
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
		StringCchPrintf(ch,50,"��������Ϊ %2.2f m",pDoc->depth[(clickpoint.x-rectClient.right/7)*720/(rectClient.right+1)]);
	else
		StringCchPrintf(ch,50,"��ѡ�������ڵ�λ��",0);
	MemDC.TextOut(rectClient.right/2-280,7*rectClient.bottom/8,ch);
	MemDC.SelectObject(blackPen);
		
	//���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ
	pDC->BitBlt(0,0,rectClient.right,rectClient.bottom,&MemDC,0,0,SRCCOPY);
	//��ͼ��ɺ������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CAUVView::OnEraseBkgnd(pDC);
}


void CCurveView::OnSize(UINT nType, int cx, int cy)
{
	CAUVView::OnSize(nType, cx, cy);
	Invalidate(FALSE);
	// TODO: �ڴ˴������Ϣ����������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//CAUVView::OnRButtonDown(nFlags, point);
}


void CCurveView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//CAUVView::OnRButtonUp(nFlags, point);
}

void CCurveView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
