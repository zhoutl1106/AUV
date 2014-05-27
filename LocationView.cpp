// CLocationView.cpp : ʵ���ļ�
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

// CLocationView ��ͼ

void CLocationView::OnDraw(CDC* pDC)
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
	MemDC.FillSolidRect(0,0,rectClient.right,rectClient.bottom,RGB(0,0,0));
	//��ͼ
	CImage image;
	CString str;
	MemDC.SetTextColor(RGB(255,234,0));
	CFont newfont;
	newfont.CreatePointFont(130,"����");
	MemDC.SelectObject(&newfont);
	str.Format("��ǰ��ת���ģ�%d",pDoc->m_origin);
	MemDC.TextOutA(0,0,str);
	image.Load("res\\images\\submarine.jpg");
	image.Draw(MemDC,rectClient.right*9/20,rectClient.bottom/2,rectClient.right/10,rectClient.right*image.GetHeight()/image.GetWidth()/10);
	
	//���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ
	pDC->BitBlt(0,0,rectClient.right,rectClient.bottom,&MemDC,0,0,SRCCOPY);
	//��ͼ��ɺ������
	image.Destroy();
	newfont.DeleteObject();
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}


// CLocationView ���

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


// CLocationView ��Ϣ�������


void CLocationView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
