// BottomView.cpp : ʵ���ļ�
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


// CBottomView ��ͼ

void CBottomView::OnDraw(CDC* pDC)
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
	//���ñ���ɫ��λͼ���
	MemDC.FillSolidRect(0,0,rectClient.right,rectClient.bottom,RGB(60,84,88));
	//��ͼ
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
	newfont.CreatePointFont(rectClient.right*130/1028,"��������");
	MemDC.SelectObject(&newfont);
	CString str;
	str.Format(" ��ز�ǰ��");
	MemDC.TextOut(0,rectClient.bottom*4/5,str);
	str.Format(" ��زֺ��");
	MemDC.TextOut(dist,rectClient.bottom*4/5,str);
	str.Format("    ������");
	MemDC.TextOut(2*dist,rectClient.bottom*4/5,str);
	str.Format("    ��չ��");   
	MemDC.TextOut(3*dist,rectClient.bottom*4/5,str);
	str.Format("   ��̬�ǲ�");
	MemDC.TextOut(6*dist,rectClient.bottom*4/5,str);
	str.Format("   �����շ�");
	MemDC.TextOut(7*dist,rectClient.bottom*4/5,str);
	str.Format("   �ŵ�״��");
	MemDC.TextOut(8*dist,rectClient.bottom*4/5,str);
	str.Format("    ������");
	MemDC.TextOut(9*dist,rectClient.bottom*4/5,str);

	//���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ
	pDC->BitBlt(0,0,rectClient.right,rectClient.bottom,&MemDC,0,0,SRCCOPY);
	//��ͼ��ɺ������
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

// CBottomView ���

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


// CBottomView ��Ϣ�������


int CBottomView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
// 	SetTimer(1,50,NULL);
	return 0;
}


void CBottomView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
// 	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}


void CBottomView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CAUVDoc *pDoc = GetDocument();
	CRect rectClient;
	GetClientRect(rectClient);
	if(point.x > rectClient.right*9/10)
		pDoc->m_AlarmSound = !pDoc->m_AlarmSound;
	CView::OnLButtonDown(nFlags, point);
}
