// CLocationView.cpp : 实现文件
//

#include "stdafx.h"
#include "AUV.h"
#include "AUVDoc.h"
#include "MainFrm.h"
#include "OpenGLView.h"
#include "CurveView.h"
#include "LocationView.h"
#include "DriveView.h"
#include "ShowView.h"
#include "CameraView.h"


// CLocationView

IMPLEMENT_DYNCREATE(CLocationView, CView)

CLocationView::CLocationView()
{

}

CLocationView::~CLocationView()
{
}

BEGIN_MESSAGE_MAP(CLocationView, CView)
	ON_WM_CHAR()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

CAUVDoc* CLocationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAUVDoc)));
	return (CAUVDoc*)m_pDocument;
}

// CLocationView 绘图

void CLocationView::OnDraw(CDC* pDC)
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
	MemDC.FillSolidRect(0,0,rectClient.right,rectClient.bottom,RGB(0,0,0));
	//绘图
	CImage image;
	CString str;
	MemDC.SetTextColor(RGB(255,234,0));
	CFont newfont;
	newfont.CreatePointFont(130,"楷体");
	MemDC.SelectObject(&newfont);
	str.Format("当前旋转中心：%d",pDoc->m_origin);
	MemDC.TextOutA(0,0,str);
	image.Load("res\\images\\submarine.jpg");
	image.Draw(MemDC,rectClient.right*9/20,rectClient.bottom/2,rectClient.right/10,rectClient.right*image.GetHeight()/image.GetWidth()/10);
	
	//将内存中的图拷贝到屏幕上进行显示
	pDC->BitBlt(0,0,rectClient.right,rectClient.bottom,&MemDC,0,0,SRCCOPY);
	//绘图完成后的清理
	image.Destroy();
	newfont.DeleteObject();
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}


// CLocationView 诊断

#ifdef _DEBUG
void CLocationView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CLocationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLocationView 消息处理程序


void CLocationView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//TODO: 在此添加消息处理程序代码和/或调用默认值
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


void CLocationView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	  //TODO: Add your message handler code here and/or call default
	  CRect rect,rectClient;
	  GetParentFrame()->GetClientRect(&rect);
	  GetClientRect(&rectClient);
	  CMainFrame* pMain = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	  if(rectClient.right < rect.right/2)
	  {     
		  pMain->m_wndSplitter.SetRowInfo(0,0,0);
		  pMain->m_wndSplitter.SetRowInfo(1,0,0);
		  pMain->m_wndSplitter.SetRowInfo(2,rect.bottom,0);
		  pMain->m_wndSplitter.SetColumnInfo(0,rect.right,0);
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
