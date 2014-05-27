// LeftView.cpp : 实现文件
//

#include "stdafx.h"
#include "AUV.h"
#include "LeftView.h"


// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CView)

CLeftView::CLeftView()
{

}

CLeftView::~CLeftView()
{
}

BEGIN_MESSAGE_MAP(CLeftView, CView)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLeftView 绘图

void CLeftView::OnDraw(CDC* pDC)
{
	CAUVDoc* pDoc = GetDocument();
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
	
	MemDC.SetTextColor(RGB(255,234,0));
	MemDC.TextOutA(0,0,"LeftView");
	CFont newfont;
	newfont.CreatePointFont(130,"华文中宋");
	MemDC.SelectObject(&newfont);
	
	CString str;
	str.Format("电池电量：%d%%",pDoc->m_Battery/100);
	MemDC.TextOutA(0,20,str);
	if(!pDoc->m_BattroomStatef)
		str.Format("电池仓前端漏水");
	else
		str.Format("电池仓前端正常");
	MemDC.TextOutA(0,40,str);
	if(!pDoc->m_BattroomStateb)
		str.Format("电池仓后端漏水");
	else
		str.Format("电池仓后端正常");
	MemDC.TextOutA(0,60,str);
	if(pDoc->m_datalost)
		str.Format("数据丢失");
	else
		str.Format("数据正常");
	MemDC.TextOut(0,80,str);
	if(pDoc->m_connetion)
		str.Format("连接正常");
	else
		str.Format("连接中断");
	MemDC.TextOut(0,100,str);
	
	//将内存中的图拷贝到屏幕上进行显示
	pDC->BitBlt(0,0,rectClient.right,rectClient.bottom,&MemDC,0,0,SRCCOPY);
	//绘图完成后的清理
	newfont.DeleteObject();
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();}

CAUVDoc* CLeftView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAUVDoc)));
	return (CAUVDoc*)m_pDocument;
}


// CLeftView 诊断

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CLeftView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLeftView 消息处理程序


int CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
// 	SetTimer(1,50,NULL);
	return 0;
}


void CLeftView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
// 	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}
