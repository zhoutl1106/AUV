
// AUVDoc.cpp : implementation of the CAUVDoc class
//
//	Name: AUVDoc.cpp
//	Function: class CAUVDoc
//	Date: Feb.8, 2011
//	Author: Tianli Zhou

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AUV.h"
#endif

#include "AUVDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAUVDoc

IMPLEMENT_DYNCREATE(CAUVDoc, CDocument)

BEGIN_MESSAGE_MAP(CAUVDoc, CDocument)
END_MESSAGE_MAP()


// CAUVDoc construction/destruction

CAUVDoc::CAUVDoc()
{
	// TODO: add one-time construction code here
	sceneRot[0] = 0;
	sceneRot[1] = 0;
	sceneRot[2] = 0;
	m_AlarmLevel = 1;
	m_Battery = 0;
	m_BattroomStatef = 0;				//0 for leak
	m_BattroomStateb = 0;				//0 for leak
	m_DeviceroomState = 0;
	m_ExtraroomState = 0;
	m_AttituderoomState = 0;
	memset(xlocation,0,sizeof(float)*MAX_COUNT_POINTS);
	memset(ylocation,0,sizeof(float)*MAX_COUNT_POINTS);
	memset(depth,0,sizeof(float)*MAX_COUNT_POINTS);
	count = 0;
	m_connetion = 0;
	m_AlarmSound = 1;
	m_errCount = 0;
}

CAUVDoc::~CAUVDoc()
{
}

BOOL CAUVDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}




// CAUVDoc serialization

void CAUVDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CAUVDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CAUVDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAUVDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CAUVDoc diagnostics

#ifdef _DEBUG
void CAUVDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAUVDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CAUVDoc commands
