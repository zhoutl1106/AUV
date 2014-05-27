// BottomView.cpp : 实现文件
//

#include "stdafx.h"
#include "AUV.h"
#include "BottomView.h"


// CBottomView

IMPLEMENT_DYNCREATE(CBottomView, CView)

CBottomView::CBottomView()
{

}

CBottomView::~CBottomView()
{
}

BEGIN_MESSAGE_MAP(CBottomView, CView)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CBottomView 绘图

void CBottomView::OnDraw(CDC* pDC)
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
	//先用背景色将位图清除
	MemDC.FillSolidRect(0,0,rectClient.right,rectClient.bottom,RGB(60,84,88));
	//绘图
	MemDC.SetStretchBltMode(COLORONCOLOR);

	MemDC.SetTextColor(RGB(255,234,0));
	

	CImage image;
	int dist = rectClient.right/10;
	pDoc->m_AlarmLevel = 2;
	//draw light of Battery State Front
	image.Destroy();
	if(pDoc->m_BattroomStatef)
		image.Load("res\\lights\\LightG.jpg");
	else
	{
		image.Load("res\\lights\\LightR.jpg");
		pDoc->m_AlarmLevel = 0;
	}
	image.Draw(MemDC,0,rectClient.bottom/2-dist/2,dist,dist);

	//draw light of Battery State Front
	image.Destroy();
	if(pDoc->m_BattroomStateb)
		image.Load("res\\lights\\LightG.jpg");
	else
	{
		image.Load("res\\lights\\LightR.jpg");
		pDoc->m_AlarmLevel = 0;
	}
	image.Draw(MemDC,dist,rectClient.bottom/2-dist/2,dist,dist);

	//draw light of Deviceroom State
	image.Destroy();
	if(pDoc->m_DeviceroomState)
		image.Load("res\\lights\\LightG.jpg");
	else
		image.Load("res\\lights\\LightR.jpg");
	image.Draw(MemDC,2*dist,rectClient.bottom/2-dist/2,dist,dist);

	//draw light of Extraroom State
	image.Destroy();
	if(pDoc->m_ExtraroomState)
		image.Load("res\\lights\\LightG.jpg");
	else
		image.Load("res\\lights\\LightR.jpg");
	image.Draw(MemDC,3*dist,rectClient.bottom/2-dist/2,dist,dist);

	//draw light of Attituderoom State
	image.Destroy();
	if(pDoc->m_AttituderoomState)
		image.Load("res\\lights\\LightG.jpg");
	else
		image.Load("res\\lights\\LightR.jpg");
	image.Draw(MemDC,6*dist,rectClient.bottom/2-dist/2,dist,dist);


	//draw light of Connect State
	image.Destroy();
	if(pDoc->m_connetion)
		image.Load("res\\lights\\connected.jpg");
	else
	{
		image.Load("res\\lights\\disconnect.jpg");
		pDoc->m_AlarmLevel = 0;
	}
	image.Draw(MemDC,8*dist,rectClient.bottom/2-dist/2,dist,dist);

	//draw light of DateLost State
	image.Destroy();
	if(pDoc->m_datalost)
	{
		image.Load("res\\lights\\dateRetransmit.jpg");
		switch(pDoc->m_AlarmLevel)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			pDoc->m_AlarmLevel = 1;
			break;
		}
	}
	else
		image.Load("res\\lights\\dateRight.jpg");
	image.Draw(MemDC,7*dist,rectClient.bottom/2-dist/2,dist,dist);

	//draw light of AlarmSound State
	image.Destroy();
	if(pDoc->m_AlarmSound)
		image.Load("res\\lights\\soundOn.jpg");
	else
	{
		image.Load("res\\lights\\soundOff.jpg");
		switch(pDoc->m_AlarmLevel)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			pDoc->m_AlarmLevel = 1;
			break;
		}
	}
	image.Draw(MemDC,9*dist,rectClient.bottom/2-dist/2,dist,dist);

	//draw light of Alarm Level
	image.Destroy();
	switch(pDoc->m_AlarmLevel)
	{
	case 0:
		image.Load("res\\lights\\Alarm.jpg");
		if(pDoc->m_AlarmSound)
			PlaySound("res\\sound\\BUZZ4.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);
		break;
	case 1:
		image.Load("res\\lights\\Warning.jpg");
		if(pDoc->m_AlarmSound)
			PlaySound("res\\sound\\BUZZ5.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);
		break;
	case 2:
		image.Load("res\\lights\\Normal.jpg");
		break;
	}
	image.Draw(MemDC,4*dist,rectClient.bottom/2-dist,2*dist,2*dist);


	CFont newfont;
	newfont.CreatePointFont(rectClient.right*130/1028,"华文中宋");
	MemDC.SelectObject(&newfont);
	CString str;
	str.Format(" 电池仓前端");
	MemDC.TextOut(0,rectClient.bottom*4/5,str);
	str.Format(" 电池仓后端");
	MemDC.TextOut(dist,rectClient.bottom*4/5,str);
	str.Format("    仪器仓");
	MemDC.TextOut(2*dist,rectClient.bottom*4/5,str);
	str.Format("    扩展仓");   
	MemDC.TextOut(3*dist,rectClient.bottom*4/5,str);
	str.Format("   姿态仪仓");
	MemDC.TextOut(6*dist,rectClient.bottom*4/5,str);
	str.Format("   数据收发");
	MemDC.TextOut(7*dist,rectClient.bottom*4/5,str);
	str.Format("   信道状况");
	MemDC.TextOut(8*dist,rectClient.bottom*4/5,str);
	str.Format("    警报音");
	MemDC.TextOut(9*dist,rectClient.bottom*4/5,str);

	//将内存中的图拷贝到屏幕上进行显示
	pDC->BitBlt(0,0,rectClient.right,rectClient.bottom,&MemDC,0,0,SRCCOPY);
	//绘图完成后的清理
	if(!image.IsNull())
		image.Destroy();
	image.Destroy();
	newfont.DeleteObject();
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}

CAUVDoc* CBottomView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAUVDoc)));
	return (CAUVDoc*)m_pDocument;
}

// CBottomView 诊断

#ifdef _DEBUG
void CBottomView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CBottomView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBottomView 消息处理程序


int CBottomView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
// 	SetTimer(1,50,NULL);
	return 0;
}


void CBottomView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
// 	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}


void CBottomView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CAUVDoc *pDoc = GetDocument();
	CRect rectClient;
	GetClientRect(rectClient);
	if(point.x > rectClient.right*9/10)
		pDoc->m_AlarmSound = !pDoc->m_AlarmSound;
	CView::OnLButtonDown(nFlags, point);
}
