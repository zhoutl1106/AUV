// ShowView.cpp : implementation file
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
#include "AUVView.h"

// CShowView

IMPLEMENT_DYNCREATE(CShowView, CView)

CShowView::CShowView()
{

}

CShowView::~CShowView()
{
}

BEGIN_MESSAGE_MAP(CShowView, CView)
//	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CHAR()
END_MESSAGE_MAP()


// CShowView drawing

void CShowView::OnDraw(CDC* pDC)
{
	CAUVDoc* pDoc = GetDocument();
	CMainFrame* pMain = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	// TODO: add draw code here
	CRect rectClient;
	GetClientRect(rectClient);
	
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
	MemDC.FillSolidRect(0,0,rectClient.right,rectClient.bottom,RGB(60,84,88));
	//绘图
	CString str;
	MemDC.SetTextColor(RGB(255,234,0));
	MemDC.TextOutA(0,0,"ShowView");
	CFont newfont;
	newfont.CreatePointFont(130,"楷体");
	MemDC.SelectObject(&newfont);
	str.Format("航向角：%3.1f",pDoc->sceneRot[1]);
	MemDC.TextOutA(0,20,str);
	str.Format("俯仰角：%3.1f",pDoc->sceneRot[0]);
	MemDC.TextOutA(0,40,str);
	str.Format("倾侧角：%3.1f",pDoc->sceneRot[2]);
	MemDC.TextOutA(0,60,str);
	str.Format("深度  ：%6.2f m",pDoc->m_depth);
	MemDC.TextOutA(0,80,str);
	str.Format("误码数 ：%d",pDoc->m_errCount);
	MemDC.TextOutA(0,100,str);
	str.Format("电池电压 ：%2.2f V",pDoc->m_Battery/100.);
	MemDC.TextOutA(0,120,str);
	str.Format("原点位置 ：%d cm",pDoc->m_origin);
	MemDC.TextOutA(0,140,str);
	//将内存中的图拷贝到屏幕上进行显示
	pDC->BitBlt(0,0,rectClient.right,rectClient.bottom,&MemDC,0,0,SRCCOPY);
	//绘图完成后的清理
	newfont.DeleteObject();
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	
}

CAUVDoc* CShowView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAUVDoc)));
	return (CAUVDoc*)m_pDocument;
}

// CShowView diagnostics

#ifdef _DEBUG
void CShowView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CShowView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CShowView message handlers

int CShowView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
// 	SetTimer(1,50,NULL);
	return 0;
}


void CShowView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
// 	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}