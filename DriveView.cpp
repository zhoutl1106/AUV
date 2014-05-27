// OpenGLView.cpp : implementation file
//

//	Name: OpenGLView.cpp
//	Function: class CDriveView
//	Date: Feb.8, 2011
//	Author: Tianli Zhou

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

// CDriveView

IMPLEMENT_DYNCREATE(CDriveView, CView)

CDriveView::CDriveView()
: mouseleftdown(FALSE)
, mouseprevpoint(0)
, mouserightdown(FALSE)
, mousemiddledown(FALSE)
{
}

CDriveView::~CDriveView()
{
}

BEGIN_MESSAGE_MAP(CDriveView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
//	ON_WM_CHAR()
ON_WM_MBUTTONDOWN()
ON_WM_MBUTTONUP()
ON_WM_MOUSEWHEEL()
ON_WM_LBUTTONDBLCLK()
ON_WM_ERASEBKGND()
ON_WM_CHAR()
END_MESSAGE_MAP()


// CDriveView drawing

void CDriveView::OnDraw(CDC* pDC)
{
	CAUVDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: add draw code here
	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	RenderScene();
	::wglMakeCurrent(m_pDC->GetSafeHdc(),NULL);	
}

CAUVDoc* CDriveView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAUVDoc)));
	return (CAUVDoc*)m_pDocument;
}

// CDriveView diagnostics

#ifdef _DEBUG
void CDriveView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CDriveView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDriveView message handlers


BOOL CDriveView::SetupPixelFormat(void)
{
	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR),    // pfd�ṹ�Ĵ�С 
	    1,                                // �汾�� 
	    PFD_DRAW_TO_WINDOW |              // ֧���ڴ����л�ͼ 
	    PFD_SUPPORT_OPENGL |              // ֧�� OpenGL 
	    PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE,// ˫����ģʽ 
	    PFD_TYPE_RGBA,                    // RGBA ��ɫģʽ 
	    16,                               // 24 λ��ɫ��� 
	    0, 0, 0, 0, 0, 0,                 // ������ɫλ 
	    0,                                // û�з�͸���Ȼ��� 
	    0,                                // ������λλ 
	    0,                                // ���ۼӻ��� 
	    0, 0, 0, 0,                       // �����ۼ�λ 
	    16,                               // 32 λ��Ȼ���     
	    0,                                // ��ģ�建�� 
	    0,                                // �޸������� 
	    PFD_MAIN_PLANE,                   // ���� 
	    0,                                // ���� 
	    0, 0, 0                           // ���Բ�,�ɼ��Ժ������ģ 
	}; 	
	int pixelformat;
	pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);//ѡ�����ظ�ʽ
	::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd);	//�������ظ�ʽ
	if(pfd.dwFlags & PFD_NEED_PALETTE)
		SetLogicalPalette();	//�����߼���ɫ��
	::wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	return TRUE;
}


void CDriveView::SetLogicalPalette(void)
{
	struct
    {
        WORD Version;
        WORD NumberOfEntries;
        PALETTEENTRY aEntries[256];
    } logicalPalette = { 0x300, 256 };

	BYTE reds[] = {0, 36, 72, 109, 145, 182, 218, 255};
	BYTE greens[] = {0, 36, 72, 109, 145, 182, 218, 255};
	BYTE blues[] = {0, 85, 170, 255};

    for (int colorNum=0; colorNum<256; ++colorNum)
    {
        logicalPalette.aEntries[colorNum].peRed =
            reds[colorNum & 0x07];
        logicalPalette.aEntries[colorNum].peGreen =
            greens[(colorNum >> 0x03) & 0x07];
        logicalPalette.aEntries[colorNum].peBlue =
            blues[(colorNum >> 0x06) & 0x03];
        logicalPalette.aEntries[colorNum].peFlags = 0;
    }

    m_hPalette = CreatePalette ((LOGPALETTE*)&logicalPalette);
}


BOOL CDriveView::RenderScene(void)
{
	CAUVDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return FALSE;
	
	if(m_baiscobj==NULL) return TRUE;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);			 // ����ˢ�±���ɫ
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);// ˢ�±���
	glLoadIdentity();								 // ���õ�ǰ��ģ�͹۲����
	//m_baiscobj->DisplayScene();						 // �����
	rad_xz = float (3.13149* -pDoc->sceneRot[1]/180.0f);	//����������ת�Ƕ�
	rad_y = float(3.13149* -pDoc->sceneRot[0]/180.0f);
	rad_xy = float(3.13149* pDoc->sceneRot[2]/180.0f);
	g_eye[2] =pDoc->m_xlocation*100-500;			//�ӵ��x����
	g_eye[0] =pDoc->m_ylocation*100;			//�ӵ��Z����
	g_eye[1] = pDoc->m_depth+600;
	g_look[0] = (float)(g_eye[0] +100*cos(rad_y)*cos(rad_xz));	//Ŀ���X����
	g_look[2] = (float)(g_eye[2] +100*cos(rad_y)*sin(rad_xz));	//Ŀ���Z����
	g_look[1] = g_eye[1] + 100*sin(rad_y);				//Ŀ���Y����
	if(-90<=pDoc->sceneRot[0] && pDoc->sceneRot[0]<=90)
		gluLookAt(g_eye[0],g_eye[1],g_eye[2],		//�ӵ�
			g_look[0],g_look[1],g_look[2],	//Ŀ���
			sin(rad_xy),cos(rad_xy),0.0						//�ӵ㷽��
			);
	else if(pDoc->sceneRot[0]>90)
		gluLookAt(g_eye[0],g_eye[1],g_eye[2],		//�ӵ�
		g_look[0],g_look[1],g_look[2],	//Ŀ���
		-sin(rad_xy),-cos(rad_xy),0.0						//�ӵ㷽��
		);
	else
		gluLookAt(g_eye[0],g_eye[1],g_eye[2],		//�ӵ�
			g_look[0],g_look[1],g_look[2],	//Ŀ���
			sin(rad_xy),-cos(rad_xy),0.0						//�ӵ㷽��
			);
	/*CString str;
	str.Format("eye: %3.1f,%3.1f,%3.1f\nlook:%3.1f,%3.1f,%3.1f\n",g_eye[0],g_eye[1],g_eye[2],g_look[0],g_look[1],g_look[2]);
	OutputDebugString(str);*/

	m_baiscobj->DrawSand();							 // ��ʾ���ɽ��
	m_baiscobj->CreateSkyBox(3,6,3,6);				 // ��ʾ��

	::SwapBuffers(m_pDC->GetSafeHdc());		//����������
	return TRUE;
}


BOOL CDriveView::InitializeOpenGL(CDC* pDC)
{
	m_pDC=pDC;
	SetupPixelFormat();
	//���ɻ���������
	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());
	//�õ�ǰ����������
	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	//gl Settings
	CRect rect;
	GetClientRect(&rect);
	glViewport(0,0,rect.Width(),rect.Height());			// ����OpenGL�ӿڴ�С��	
	glMatrixMode(GL_PROJECTION);			// ���õ�ǰ����ΪͶӰ����
	glLoadIdentity();						// ���õ�ǰָ���ľ���Ϊ��λ����
	gluPerspective							// ����͸��ͼ
		( 54.0f,							// ͸�ӽ�����Ϊ 45 ��
		(GLfloat)rect.Width()/(GLfloat)rect.Height(),	// ���ڵĿ���߱�
		0.1f,								// ��Ұ͸�����:����1.0f
		3000.0f							// ��Ұ͸�����:ʼ��0.1fԶ��1000.0f
		);
	// �������������ƣ���һ���������þ�ͷ��Ƕȣ��ڶ��������ǳ���ȣ�������Զ�����С�
	glMatrixMode(GL_MODELVIEW);				// ���õ�ǰ����Ϊģ����ͼ����
	glLoadIdentity();						// ���õ�ǰָ���ľ���Ϊ��λ����
	//====================================================
	m_baiscobj=new baiscobj();
	m_baiscobj->light0();
	return TRUE;
}

int CDriveView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pDC = new CClientDC(this);
// 	SetTimer(1,50,NULL);
	camPos[0]=0;
	camPos[1]=0;
	camPos[2]=-1000;
	camRot[0]=30;
	camRot[1]=135;
	camRot[2]=0;
	InitializeOpenGL(m_pDC);
	return 0;
}


void CDriveView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	if(m_pDC != 0)
	{
		::wglMakeCurrent(m_pDC->GetSafeHdc(),NULL);
		m_pDC->ReleaseOutputDC();
	}
	if(m_hRC != 0)
	{
		::wglDeleteContext(m_hRC);
		m_hRC = 0;
	}
}


void CDriveView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	// TODO: Add your message handler code here
	/////////////////////////////////////////////////////////////////
//��Ӵ�������ʱ��ͼ�α任����
	glViewport(0,0,cx,cy);
/////////////////////////////////////////////////////////////////
	GLdouble aspect_ratio;
	aspect_ratio = (GLdouble)cx/(GLdouble)cy;
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	gluPerspective(40.0F, aspect_ratio, 1.0F, 10000.0F);
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	::wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
}


BOOL CDriveView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	return CView::PreCreateWindow(cs);
}

void CDriveView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
// 	Invalidate(FALSE);		
	CView::OnTimer(nIDEvent);
}


void CDriveView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SetCapture( );
	mouseleftdown = TRUE;
	mouseprevpoint.x = point.x;
	mouseprevpoint.y = point.y;	
	//CView::OnLButtonDown(nFlags, point);
}


void CDriveView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture( );
	mouseleftdown = FALSE;
	SetCamRot(0, (point.y - mouseprevpoint.y) , TRUE, TRUE);
	SetCamRot(1, (point.x - mouseprevpoint.x) , TRUE, TRUE);
	//CView::OnLButtonUp(nFlags, point);
}


void CDriveView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(mouserightdown)
	{
		//SetCamRot(1, (point.x - mouseprevpoint.x) , TRUE,TRUE);
	}
	else if(mouseleftdown)
	{	
		SetCamRot(0, (point.y - mouseprevpoint.y) , TRUE,TRUE);
		SetCamRot(1, (point.x - mouseprevpoint.x) , TRUE,TRUE);
	}
	else if(mousemiddledown)
	{
		//SetCamPos(1,-(point.y - mouseprevpoint.y),TRUE,TRUE);
		//SetCamPos(0,(point.x - mouseprevpoint.x),TRUE,TRUE);
	}
	mouseprevpoint.x = point.x;
	mouseprevpoint.y = point.y;
	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	RenderScene();
	::wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	//CView::OnMouseMove(nFlags, point);
}


void CDriveView::SetSceneRot(int axis, int value, BOOL increment, BOOL apply)
{
	CAUVDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if(increment)
		pDoc->sceneRot[axis] += (pDoc->sceneRot[axis] >=360) ? (-360 + value/2): value/2;
	else
		pDoc->sceneRot[axis] = (pDoc->sceneRot[axis] >=360) ? (-360 + value/2): value/2;
}


void CDriveView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SetCapture( );
	mouserightdown = TRUE;
	mouseprevpoint.x = point.x;
	mouseprevpoint.y = point.y;		
	//CView::OnRButtonDown(nFlags, point);
}


void CDriveView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture( );
	mouserightdown = FALSE;
	//SetCamRot(1, (point.x - mouseprevpoint.x) , TRUE, TRUE);
	//CView::OnRButtonUp(nFlags, point);
}


void CDriveView::SetCamPos(int axis, int value, BOOL increment, bool apply)
{
	if(increment)
	{
		camPos[axis] += (float)value;
	}
	else
	{
		camPos[axis] = (float)value/2;
	}
// 	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
// 	::wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
}

void CDriveView::SetCamRot(int axis, int value, BOOL increment, BOOL apply)
{
	if(increment)
		camRot[axis] += (camRot[axis] >=360) ? (-360 + value/2): value/2;
	else
		camRot[axis] = (camRot[axis] >=360) ? (-360 + value/2): value/2;
}

void CDriveView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SetCapture( );
	mousemiddledown = TRUE;
	mouseprevpoint.x = point.x;
	mouseprevpoint.y = point.y;	
	//CView::OnMButtonDown(nFlags, point);
}


void CDriveView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture( );
	mousemiddledown = FALSE;
	//SetCamPos(1,-(point.y - mouseprevpoint.y),TRUE,TRUE);
	//SetCamPos(0,(point.x - mouseprevpoint.x),TRUE,TRUE);
	//CView::OnMButtonUp(nFlags, point);
}


BOOL CDriveView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if(zDelta < 0)
	{
		SetCamPos(2,zDelta/10,TRUE,TRUE);
	}
	else if(zDelta>0)
	{
		SetCamPos(2,zDelta/10,TRUE,TRUE);
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CDriveView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect,rectClient;
	GetParentFrame()->GetClientRect(&rect);
	GetClientRect(&rectClient);
	CMainFrame* pMain = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	if(rectClient.right < rect.right*6/7)
	{     
		pMain->m_wndSplitter.SetRowInfo(0,0,0);
		pMain->m_wndSplitter.SetRowInfo(1,rect.bottom,0);
		pMain->m_wndSplitter.SetColumnInfo(0,0,0);
		pMain->m_wndSplitter.SetColumnInfo(1,rect.right,0);
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


BOOL CDriveView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}

void CDriveView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

