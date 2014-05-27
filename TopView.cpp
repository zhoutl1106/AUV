// TopView.cpp : ʵ���ļ�
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


// CTopView ��ͼ

void CTopView::OnDraw(CDC* pDC)
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
	MemDC.FillSolidRect(0,0,rectClient.right,rectClient.bottom,RGB(60,84,88));
	//��ͼ
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
	//���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ
	pDC->BitBlt(0,0,rectClient.right,rectClient.bottom,&MemDC,0,0,SRCCOPY);
	//��ͼ��ɺ������
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

// CTopView ���

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


// CTopView ��Ϣ�������


int CTopView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
// 	SetTimer(1,50,NULL);
	pointTime.SetPoint(1000,41);
	pointMillisecond.SetPoint(1000,61);
	pointDate.SetPoint(100,80);

	return 0;
}


void CTopView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
// 	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}


void CTopView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

}
