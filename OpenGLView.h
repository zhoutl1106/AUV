#pragma once
#include "atltypes.h"
#include "AUVDoc.h"

// COpenGLView view

//	Name: OpenGLView.h
//	Function: header file of OpenGL View Class
//	Date: Feb.8, 2011
//	Author: Tianli Zhou

class COpenGLView : public CView
{
	DECLARE_DYNCREATE(COpenGLView)

protected:
	COpenGLView();           // protected constructor used by dynamic creation
	virtual ~COpenGLView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

	HGLRC		m_hRC;			//OpenGL绘制描述表
	HPALETTE	m_hPalette;		//OpenGL调色板
	CDC*		m_pDC;			//OpenGL设备描述表
	int			Model;
	
	float		camRot[3];
	float		camPos[3];
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL SetupPixelFormat(void);
	void SetLogicalPalette(void);
	BOOL RenderScene(void);
	BOOL InitializeOpenGL(CDC* pDC);
	CAUVDoc* GetDocument() const;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL mouseleftdown;
	BOOL mousemiddledown;
	CPoint mouseprevpoint;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void SetSceneRot(int axis, int value, BOOL increment, BOOL apply);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	BOOL mouserightdown;
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	void SetCamPos(int axis, int value, BOOL increment, bool apply);
	int Axis;
	void SetCamRot(int axis, int value, BOOL increment, BOOL apply);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


