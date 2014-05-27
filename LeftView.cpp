// LeftView.cpp : ʵ���ļ�
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


// CLeftView ��ͼ

void CLeftView::OnDraw(CDC* pDC)
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
	
	MemDC.SetTextColor(RGB(255,234,0));
	MemDC.TextOutA(0,0,"LeftView");
	CFont newfont;
	newfont.CreatePointFont(130,"��������");
	MemDC.SelectObject(&newfont);
	
	CString str;
	str.Format("��ص�����%d%%",pDoc->m_Battery/100);
	MemDC.TextOutA(0,20,str);
	if(!pDoc->m_BattroomStatef)
		str.Format("��ز�ǰ��©ˮ");
	else
		str.Format("��ز�ǰ������");
	MemDC.TextOutA(0,40,str);
	if(!pDoc->m_BattroomStateb)
		str.Format("��زֺ��©ˮ");
	else
		str.Format("��زֺ������");
	MemDC.TextOutA(0,60,str);
	if(pDoc->m_datalost)
		str.Format("���ݶ�ʧ");
	else
		str.Format("��������");
	MemDC.TextOut(0,80,str);
	if(pDoc->m_connetion)
		str.Format("��������");
	else
		str.Format("�����ж�");
	MemDC.TextOut(0,100,str);
	
	//���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ
	pDC->BitBlt(0,0,rectClient.right,rectClient.bottom,&MemDC,0,0,SRCCOPY);
	//��ͼ��ɺ������
	newfont.DeleteObject();
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();}

CAUVDoc* CLeftView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAUVDoc)));
	return (CAUVDoc*)m_pDocument;
}


// CLeftView ���

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


// CLeftView ��Ϣ�������


int CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
// 	SetTimer(1,50,NULL);
	return 0;
}


void CLeftView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
// 	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}
