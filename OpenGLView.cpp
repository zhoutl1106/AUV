// OpenGLView.cpp : implementation file
//

//	Name: OpenGLView.cpp
//	Function: class COpenGLView
//	Date: Feb.8, 2011
//	Author: Tianli Zhou

#include "stdafx.h"
#include "AUV.h"
#include "dc10.h"
#include "axis.h"
#include "AUVDoc.h"
#include "MainFrm.h"
#include "OpenGLView.h"
#include "CurveView.h"
#include "LocationView.h"
#include "DriveView.h"
#include "ShowView.h"
#include "CameraView.h"

// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

COpenGLView::COpenGLView()
: mouseleftdown(FALSE)
, mouseprevpoint(0)
, mouserightdown(FALSE)
, mousemiddledown(FALSE)
{

	Axis = 0;
}

COpenGLView::~COpenGLView()
{
}

BEGIN_MESSAGE_MAP(COpenGLView, CView)
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


// COpenGLView drawing

void COpenGLView::OnDraw(CDC* pDC)
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

CAUVDoc* COpenGLView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAUVDoc)));
	return (CAUVDoc*)m_pDocument;
}

// COpenGLView diagnostics

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// COpenGLView message handlers


BOOL COpenGLView::SetupPixelFormat(void)
{
	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR),    // pfd结构的大小 
	    1,                                // 版本号 
	    PFD_DRAW_TO_WINDOW |              // 支持在窗口中绘图 
	    PFD_SUPPORT_OPENGL |              // 支持 OpenGL 
	    PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE,// 双缓存模式 
	    PFD_TYPE_RGBA,                    // RGBA 颜色模式 
	    24,                               // 24 位颜色深度 
	    0, 0, 0, 0, 0, 0,                 // 忽略颜色位 
	    0,                                // 没有非透明度缓存 
	    0,                                // 忽略移位位 
	    0,                                // 无累加缓存 
	    0, 0, 0, 0,                       // 忽略累加位 
	    32,                               // 32 位深度缓存     
	    0,                                // 无模板缓存 
	    0,                                // 无辅助缓存 
	    PFD_MAIN_PLANE,                   // 主层 
	    0,                                // 保留 
	    0, 0, 0                           // 忽略层,可见性和损毁掩模 
	}; 	
	int pixelformat;
	pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);//选择像素格式
	::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd);	//设置像素格式
	if(pfd.dwFlags & PFD_NEED_PALETTE)
		SetLogicalPalette();	//设置逻辑调色板
	::wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	return TRUE;
}


void COpenGLView::SetLogicalPalette(void)
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


BOOL COpenGLView::RenderScene(void)
{
	CAUVDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return FALSE;
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	
	::glPushMatrix();
	::glTranslatef( camPos[0], camPos[1], camPos[2] );
	::glRotatef( camRot[0], 1.0F, 0.0F, 0.0F );
	::glRotatef( camRot[1], 0.0F, 1.0F, 0.0F );
	::glRotatef( camRot[2], 0.0F, 0.0F, 1.0F );
	glCallList(Axis);
	::glTranslatef(-30,30,-30);
	::glRotatef( pDoc->sceneRot[0], 1.0F, 0.0F, 0.0F );
	::glRotatef( pDoc->sceneRot[1], 0.0F, 1.0F, 0.0F );
	::glRotatef( pDoc->sceneRot[2], 0.0F, 0.0F, 1.0F );
	glScalef(0.005f,0.005f,0.005f);
	glEnable(GL_DEPTH_TEST);
	glCallList(Model);
	glDisable(GL_DEPTH_TEST);
	::glPopMatrix();
	
	::SwapBuffers(m_pDC->GetSafeHdc());		//交互缓冲区
	return TRUE;
}


BOOL COpenGLView::InitializeOpenGL(CDC* pDC)
{
	m_pDC=pDC;
	SetupPixelFormat();
	//生成绘制描述表
	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());
	//置当前绘制描述表
	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	//gl Settings
	::glShadeModel(GL_SMOOTH);
	::glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
	//::glClearColor(1.0F, 1.0F, 1.0F, 1.0F);
	::glClearDepth(1.0F);
	 //glEnable(GL_DEPTH_TEST);
	 glDepthFunc(GL_LEQUAL);
	 glEnable(GL_CULL_FACE);
	 glCullFace(GL_BACK);
	 glFrontFace(GL_CCW);
	 glDisable(GL_DITHER);

	GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f};
	GLfloat diffuseLight[] = { 0.05f, 0.05f, 0.05f, 1.0f};
	GLfloat lightPos[]     = { 0.0f, 200.0f, 200.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat no_mat[] = { 0.5, 0.5, 0.8, 1.0 };
    glMaterialfv(GL_FRONT, GL_FRONT_AND_BACK, no_mat);
	
	Model = GL3DS_initialize_dc10();
	Axis = GL3DS_initialize_axis();

	return TRUE;
}

int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pDC = new CClientDC(this);
// 	SetTimer(1,50,NULL);
	camPos[0]=0;
	camPos[1]=0;
	camPos[2]=-250;
	camRot[0]=30;
	camRot[1]=135;
	camRot[2]=0;
	InitializeOpenGL(m_pDC);
	return 0;
}


void COpenGLView::OnDestroy()
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


void COpenGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	// TODO: Add your message handler code here
	/////////////////////////////////////////////////////////////////
//添加窗口缩放时的图形变换函数
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


BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	return CView::PreCreateWindow(cs);
}

void COpenGLView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
// 	Invalidate(FALSE);
		
	CView::OnTimer(nIDEvent);
}


void COpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SetCapture( );
	mouseleftdown = TRUE;
	mouseprevpoint.x = point.x;
	mouseprevpoint.y = point.y;	
	//CView::OnLButtonDown(nFlags, point);
}


void COpenGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture( );
	mouseleftdown = FALSE;
	SetCamRot(0, (point.y - mouseprevpoint.y) , TRUE, TRUE);
	SetCamRot(1, (point.x - mouseprevpoint.x) , TRUE, TRUE);
	//CView::OnLButtonUp(nFlags, point);
}


void COpenGLView::OnMouseMove(UINT nFlags, CPoint point)
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


void COpenGLView::SetSceneRot(int axis, int value, BOOL increment, BOOL apply)
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


void COpenGLView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SetCapture( );
	mouserightdown = TRUE;
	mouseprevpoint.x = point.x;
	mouseprevpoint.y = point.y;		
	//CView::OnRButtonDown(nFlags, point);
}


void COpenGLView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture( );
	mouserightdown = FALSE;
	//SetCamRot(1, (point.x - mouseprevpoint.x) , TRUE, TRUE);
	//CView::OnRButtonUp(nFlags, point);
}


void COpenGLView::SetCamPos(int axis, int value, BOOL increment, bool apply)
{
	if(increment)
	{
		camPos[axis] += (float)value;
	}
	else
	{
		camPos[axis] = (float)value/2;
	}
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	::wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
}

void COpenGLView::SetCamRot(int axis, int value, BOOL increment, BOOL apply)
{
	if(increment)
		camRot[axis] += (camRot[axis] >=360) ? (-360 + value/2): value/2;
	else
		camRot[axis] = (camRot[axis] >=360) ? (-360 + value/2): value/2;
}

void COpenGLView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SetCapture( );
	mousemiddledown = TRUE;
	mouseprevpoint.x = point.x;
	mouseprevpoint.y = point.y;	
	//CView::OnMButtonDown(nFlags, point);
}


void COpenGLView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture( );
	mousemiddledown = FALSE;
	//SetCamPos(1,-(point.y - mouseprevpoint.y),TRUE,TRUE);
	//SetCamPos(0,(point.x - mouseprevpoint.x),TRUE,TRUE);
	//CView::OnMButtonUp(nFlags, point);
}


BOOL COpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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

void COpenGLView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect,rectClient;
	GetParentFrame()->GetClientRect(&rect);
	GetClientRect(&rectClient);
	CMainFrame* pMain = (CMainFrame* )AfxGetApp()->m_pMainWnd;
	if(rectClient.right < rect.right/2)
	{     
		pMain->m_wndSplitter.SetRowInfo(0,rect.bottom,0);
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


BOOL COpenGLView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}

void COpenGLView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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