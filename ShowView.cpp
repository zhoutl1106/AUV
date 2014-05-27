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
	CString str;
	MemDC.SetTextColor(RGB(255,234,0));
	MemDC.TextOutA(0,0,"ShowView");
	CFont newfont;
	newfont.CreatePointFont(130,"����");
	MemDC.SelectObject(&newfont);
	str.Format("����ǣ�%3.1f",pDoc->sceneRot[1]);
	MemDC.TextOutA(0,20,str);
	str.Format("�����ǣ�%3.1f",pDoc->sceneRot[0]);
	MemDC.TextOutA(0,40,str);
	str.Format("���ǣ�%3.1f",pDoc->sceneRot[2]);
	MemDC.TextOutA(0,60,str);
	str.Format("���  ��%6.2f m",pDoc->m_depth);
	MemDC.TextOutA(0,80,str);
	str.Format("������ ��%d",pDoc->m_errCount);
	MemDC.TextOutA(0,100,str);
	str.Format("��ص�ѹ ��%2.2f V",pDoc->m_Battery/100.);
	MemDC.TextOutA(0,120,str);
	str.Format("ԭ��λ�� ��%d cm",pDoc->m_origin);
	MemDC.TextOutA(0,140,str);
	//���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ
	pDC->BitBlt(0,0,rectClient.right,rectClient.bottom,&MemDC,0,0,SRCCOPY);
	//��ͼ��ɺ������
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

	// TODO:  �ڴ������ר�õĴ�������
// 	SetTimer(1,50,NULL);
	return 0;
}


void CShowView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
// 	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}