// TopView.cpp : 实现文件
//

#include "stdafx.h"
#include "AUV.h"
#include "TopView.h"


// CTopView

IMPLEMENT_DYNCREATE(CTopView, CView)

CTopView::CTopView()
{

}

CTopView::~CTopView()
{
}

BEGIN_MESSAGE_MAP(CTopView, CView)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CHAR()
END_MESSAGE_MAP()


// CTopView 绘图

void CTopView::OnDraw(CDC* pDC)
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
	MemDC.SetStretchBltMode(COLORONCOLOR);
	MemDC.SetBkMode(TRANSPARENT);
					
	CFont fontDate,fontTime,fontMillisecond;
	fontTime.CreatePointFont(rectClient.right*480/1028,"LED");
	fontMillisecond.CreatePointFont(rectClient.right*320/1028,"LED");
	fontDate.CreatePointFont(rectClient.right*150/1028,"LED");
	//draw left
	CImage image;
	if(pDoc->m_AlarmLevel == 0)
	{
		image.Destroy();
		image.Load("res\\images\\frameEL.jpg");
		MemDC.SetTextColor(RGB(254,18,5));
	}
	else
	{
		image.Destroy();
		image.Load("res\\images\\frameNL.jpg");
		MemDC.SetTextColor(RGB(250,231,5));
	}
	image.Draw(MemDC,0,0,rectClient.right/2,rectClient.bottom);
					
	SYSTEMTIME   t; 
	GetLocalTime(&t); 
	CString strDate,strTime,strMillisecond,strTimeleft;
	strDate.Format("%d.%02d.%02d",t.wYear,t.wMonth,t.wDay);
	strTime.Format(" %02d:%02d:%02d",t.wHour,t.wMinute,t.wSecond);
	strMillisecond.Format(":%02d",t.wMilliseconds/10);
	MemDC.SelectObject(&fontTime);
	MemDC.TextOutA(rectClient.right*76/1028,rectClient.bottom*51/171,strTime);
	MemDC.SelectObject(&fontMillisecond);
	MemDC.TextOutA(rectClient.right*316/1028,rectClient.bottom*68/171,strMillisecond);
	MemDC.SelectObject(&fontDate);
	MemDC.TextOutA(rectClient.right*420/1028,rectClient.bottom*137/171,strDate);
					
	switch(pDoc->m_Battery/2000)
	{
	case 5:
	case 4:		
		image.Destroy();
		image.Load("res\\images\\BATTERY4.bmp");
		break;
	case 3:		
		image.Destroy();
		image.Load("res\\images\\BATTERY3.bmp");
		break;
	case 2:
		image.Destroy();
		image.Load("res\\images\\BATTERY2.bmp");
		break;
	case 1:
		image.Destroy();
		image.Load("res\\images\\BATTERY1.bmp");
		break;
	case 0:
		image.Destroy();
		image.Load("res\\images\\BATTERY0.bmp");
		break;
	}
	image.Draw(MemDC,rectClient.right*409/1028,rectClient.bottom*42/171,rectClient.right*95/1028,rectClient.bottom*51/171);
					
	//draw right
	if(pDoc->m_Battery < 5)
	{
		image.Destroy();
		image.Load("res\\images\\frameER.jpg");
		MemDC.SetTextColor(RGB(254,18,5));
	}
	else
	{
		image.Destroy();
		image.Load("res\\images\\frameNR.jpg");
		MemDC.SetTextColor(RGB(250,231,5));
	}
	image.Draw(MemDC,rectClient.right/2,0,rectClient.right/2,rectClient.bottom);
					
	strTimeleft.Format("02:33:01");
	MemDC.SelectObject(&fontTime);
	MemDC.TextOutA(rectClient.right*654/1028,rectClient.bottom*51/171,strTimeleft);
	image.Destroy();
	image.Load("res\\images\\inside.jpg");
	image.Draw(MemDC,rectClient.right*937/1028,rectClient.bottom*41/171,rectClient.right*77/1028,rectClient.bottom*28/171);
	image.Destroy();
	image.Load("res\\images\\main.jpg");
	image.Draw(MemDC,rectClient.right*937/1028,rectClient.bottom*70/171,rectClient.right*77/1028,rectClient.bottom*19/171);
	CString str;
// 	str.Format("%d,%d %d,%d %d,%d,%d,%d\n",pointTime.x,pointTime.y,pointMillisecond.x,pointMillisecond.y,pointDate.x,pointDate.y,rectClient.Width(),pDoc->m_AlarmLevel);
// 	OutputDebugString(str);
	//将内存中的图拷贝到屏幕上进行显示
	pDC->BitBlt(0,0,rectClient.right,rectClient.bottom,&MemDC,0,0,SRCCOPY);
	//绘图完成后的清理
	if(!image.IsNull())
		image.Destroy();
	fontDate.DeleteObject();
	fontMillisecond.DeleteObject();
	fontTime.DeleteObject();
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();	
}

CAUVDoc* CTopView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAUVDoc)));
	return (CAUVDoc*)m_pDocument;
}

// CTopView 诊断

#ifdef _DEBUG
void CTopView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CTopView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTopView 消息处理程序


int CTopView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
// 	SetTimer(1,50,NULL);
	pointTime.SetPoint(1000,41);
	pointMillisecond.SetPoint(1000,61);
	pointDate.SetPoint(100,80);

	return 0;
}


void CTopView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
// 	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}


void CTopView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

}
