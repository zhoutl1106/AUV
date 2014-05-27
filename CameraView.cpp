// DriveView.cpp : implementation file
//

#include "stdafx.h"
#include "AUV.h"
#include "MainFrm.h"
#include "AUVDoc.h"
#include "OpenGLView.h"
#include "CurveView.h"
#include "LocationView.h"
#include "DriveView.h"
#include "ShowView.h"
#include "CameraView.h"

// CCameraView

IMPLEMENT_DYNCREATE(CCameraView, CView)

CCameraView::CCameraView()
{
	mouseleftdown = FALSE;
	zoom = FALSE;
	DrawZoom = FALSE;
	mousestartpoint.x = 0;
	mousestartpoint.y = 0;
	mouseendpoint.x = 100;
	mouseendpoint.y = 100;
	m_model = FALSE;
}

CCameraView::~CCameraView()
{
}

BEGIN_MESSAGE_MAP(CCameraView, CView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CHAR()
END_MESSAGE_MAP()


// CCameraView drawing

void CCameraView::OnDraw(CDC* pDC)
{
	CAUVDoc* pDoc = GetDocument();
	// TODO: add draw code here
	CRect rectClient;
	GetClientRect(rectClient);

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
	CBitmap bitmap;
	bitmap.LoadBitmapA(IDB_SS);
	CBrush bitmapBrush(&bitmap);
	MemDC.FillRect(CRect(0,0,rectClient.right,rectClient.bottom),&bitmapBrush);
	//��ͼ
	MemDC.SetBkMode(TRANSPARENT);
	CFont newFont;
	newFont.CreateFont(rectClient.bottom/8,rectClient.bottom/14,0,0,0,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH|FF_MODERN,"����");
	CFont *oldFont;
	oldFont = MemDC.SelectObject(&newFont);

	// draw top view
	CPen redPen(PS_SOLID,1,RGB(255,0,0));
	CPen blackPen(PS_SOLID,2,RGB(0,0,0));
	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush *pOldBrush = MemDC.SelectObject(pBrush);
	bitmap.LoadBitmap(IDB_GRID);
	bitmapBrush.CreatePatternBrush(&bitmap);
	MemDC.SelectObject(bitmapBrush);
	if(!m_model)
	{
		MemDC.SelectObject(blackPen);
		MemDC.Rectangle(rectClient.right/3,rectClient.bottom/8,rectClient.right/3+3*rectClient.bottom/8,7*rectClient.bottom/8);

		int i;
		for(i = pDoc->count+1;i<MAX_COUNT_POINTS-1;i++)
		{
			if(pDoc->xlocation[i+1] != 0 && pDoc->ylocation[i+1] != 0 && pDoc->xlocation[i] != 0 && pDoc->ylocation[i] != 0)
			{
				MemDC.MoveTo(rectClient.right/3+(int)(pDoc->xlocation[i]*rectClient.bottom*3/28.8),rectClient.bottom*7/8-(int)(pDoc->ylocation[i]*rectClient.bottom*3/28.8));
				MemDC.LineTo(rectClient.right/3+(int)(pDoc->xlocation[i+1]*rectClient.bottom*3/28.8),rectClient.bottom*7/8-(int)(pDoc->ylocation[i+1]*rectClient.bottom*3/28.8));
			}
		}
		if(pDoc->xlocation[0] != 0 && pDoc->ylocation[0] != 0 && pDoc->xlocation[pDoc->count-1] != 0 && pDoc->ylocation[pDoc->count-1] != 0)
		{
			MemDC.MoveTo(rectClient.right/3+(int)(pDoc->xlocation[MAX_COUNT_POINTS-1]*rectClient.bottom*3/28.8),rectClient.bottom*7/8-(int)(pDoc->ylocation[MAX_COUNT_POINTS-1]*rectClient.bottom*3/28.8));
			MemDC.LineTo(rectClient.right/3+(int)(pDoc->xlocation[0]*rectClient.bottom*3/28.8),rectClient.bottom*7/8-(int)(pDoc->ylocation[0]*rectClient.bottom*3/28.8));
		}
		for(i = 0;i<pDoc->count-1;i++)
		{
			if(pDoc->xlocation[i+1] != 0 && pDoc->ylocation[i+1] != 0 && pDoc->xlocation[i] != 0 && pDoc->ylocation[i] != 0)
			{
				MemDC.MoveTo(rectClient.right/3+(int)(pDoc->xlocation[i]*rectClient.bottom*3/28.8),rectClient.bottom*7/8-(int)(pDoc->ylocation[i]*rectClient.bottom*3/28.8));
				MemDC.LineTo(rectClient.right/3+(int)(pDoc->xlocation[i+1]*rectClient.bottom*3/28.8),rectClient.bottom*7/8-(int)(pDoc->ylocation[i+1]*rectClient.bottom*3/28.8));
			}
		}
		MemDC.FillSolidRect(rectClient.right/3+(int)(pDoc->m_xlocation*rectClient.bottom*3/28.8),rectClient.bottom*7/8-(int)(pDoc->m_ylocation*rectClient.bottom*3/28.8),
			6,6,RGB(255,255,0));
	}
	else
	{
		MemDC.SelectObject(blackPen);
		int ox = rectClient.right/9;
		int oy = rectClient.bottom/2-rectClient.right/6;
		MemDC.Rectangle(ox,oy,ox+rectClient.right*2/3,oy+rectClient.right/3);

		int i;
		for(i = pDoc->count+1;i<MAX_COUNT_POINTS-1;i++)
		{
			if(pDoc->xlocation[i+1] != 0 && pDoc->ylocation[i+1] != 0 && pDoc->xlocation[i] != 0 && pDoc->ylocation[i] != 0)
			{
				MemDC.MoveTo(ox+(int)(pDoc->ylocation[i]*rectClient.right*2/3/7.2),oy+(int)(pDoc->xlocation[i]*rectClient.right/3/3.6));
				MemDC.LineTo(ox+(int)(pDoc->ylocation[i+1]*rectClient.right*2/3/7.2),oy+(int)(pDoc->xlocation[i+1]*rectClient.right/3/3.6));
			}
		}
		if(pDoc->xlocation[0] != 0 && pDoc->ylocation[0] != 0 && pDoc->xlocation[pDoc->count] != 0 && pDoc->ylocation[pDoc->count] != 0)
		{
			MemDC.MoveTo(ox+(int)(pDoc->ylocation[MAX_COUNT_POINTS-1]*rectClient.right*2/3/7.2),oy+(int)(pDoc->xlocation[MAX_COUNT_POINTS-1]*rectClient.right/3/3.6));
			MemDC.LineTo(ox+(int)(pDoc->ylocation[0]*rectClient.right*2/3/7.2),oy+(int)(pDoc->xlocation[0]*rectClient.right/3/3.6));
		}
		for(i = 0;i<pDoc->count-1;i++)
		{
			if(pDoc->xlocation[i+1] != 0 && pDoc->ylocation[i+1] != 0 && pDoc->xlocation[i] != 0 && pDoc->ylocation[i] != 0)
			{
				MemDC.MoveTo(ox+(int)(pDoc->ylocation[i]*rectClient.right*2/3/7.2),oy+(int)(pDoc->xlocation[i]*rectClient.right/3/3.6));
				MemDC.LineTo(ox+(int)(pDoc->ylocation[i+1]*rectClient.right*2/3/7.2),oy+(int)(pDoc->xlocation[i+1]*rectClient.right/3/3.6));
			}
		}
		MemDC.FillSolidRect(ox+(int)(pDoc->m_ylocation*rectClient.right*2/3/7.2),oy+(int)(pDoc->m_xlocation*rectClient.right/3/3.6),
			6,6,RGB(255,255,0));
	}
	//Rectangle for zoom
	if(DrawZoom)
	{
		MemDC.SelectObject(&redPen);
		MemDC.SelectObject(pBrush);
		MemDC.Rectangle(CRect(mousestartpoint,mouseendpoint));
		MemDC.SelectObject(pOldBrush);
	}

	//text output

//���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ
	if(!zoom)
	{
		pDC->BitBlt(0,0,rectClient.right,rectClient.bottom,&MemDC,0,0,SRCCOPY);
	}
	else
	{	
		if((mouseendpoint.x-mousestartpoint.x)/(mouseendpoint.y-mousestartpoint.y)>(rectClient.right/rectClient.bottom))
			pDC->StretchBlt(0,0,rectClient.right,rectClient.bottom,&MemDC,
				mousestartpoint.x,mousestartpoint.y,mouseendpoint.x-mousestartpoint.x,(mouseendpoint.x-mousestartpoint.x)*rectClient.bottom/rectClient.right,SRCCOPY);
		else
			pDC->StretchBlt(0,0,rectClient.right,rectClient.bottom,&MemDC,
				mousestartpoint.x,mousestartpoint.y,(mouseendpoint.y-mousestartpoint.y)*rectClient.right/rectClient.bottom,mouseendpoint.y-mousestartpoint.y,SRCCOPY);
	}
	
	//��ͼ��ɺ������

	pOldBrush->DeleteObject();
	pBrush->DeleteObject();
	blackPen.DeleteObject();
	redPen.DeleteObject();
	newFont.DeleteObject();
	bitmapBrush.DeleteObject();
	bitmap.DeleteObject();
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}

CAUVDoc* CCameraView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAUVDoc)));
	return (CAUVDoc*)m_pDocument;
}

// CCameraView diagnostics

#ifdef _DEBUG
void CCameraView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCameraView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCameraView message handlers


void CCameraView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect,rectClient;
	GetParentFrame()->GetClientRect(&rect);
	GetClientRect(&rectClient);
	CMainFrame* pMain = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	if(rectClient.right < rect.right/2)
	{     
		pMain->m_wndSplitter.SetRowInfo(0,0,0);
		pMain->m_wndSplitter.SetRowInfo(1,0,0);
		pMain->m_wndSplitter.SetRowInfo(2,rect.bottom,0);
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
	//CView::OnLButtonDblClk(nFlags, point);
}


void CCameraView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!zoom)
	{
		SetCapture( );
		mouseleftdown = TRUE;
		DrawZoom = TRUE;
		mousestartpoint.x = point.x;
		mousestartpoint.y = point.y;	
	}
	CView::OnLButtonDown(nFlags, point);
}


void CCameraView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!zoom)
	{
		ReleaseCapture( );
		mouseleftdown = FALSE;
		if(mouseendpoint.y != mousestartpoint.y && mouseendpoint.x != mousestartpoint.x)
		{
			mouseendpoint.x = point.x;
			mouseendpoint.y = point.y;
		}
	}
	CView::OnLButtonUp(nFlags, point);
}


void CCameraView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(mouseleftdown)
	{
		mouseendpoint.x = point.x;
		mouseendpoint.y = point.y;
	}
	CView::OnMouseMove(nFlags, point);
}


void CCameraView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	zoom = !zoom;
	DrawZoom = !DrawZoom;
	CView::OnRButtonUp(nFlags, point);
}


BOOL CCameraView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}

extern CCameraView *pCameraView;

int CCameraView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
// 	SetTimer(1,50,NULL);
	return 0;
}


void CCameraView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
// 	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}


void CCameraView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
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


