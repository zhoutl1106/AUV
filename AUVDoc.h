
// AUVDoc.h : interface of the CAUVDoc class
//
//	Name: AUVDoc.h
//	Function: class CAUVDoc
//	Date: Feb.8, 2011
//	Author: Tianli Zhou

#pragma once

#define MAX_COUNT_POINTS 400

class CAUVDoc : public CDocument
{
protected: // create from serialization only
	CAUVDoc();
	DECLARE_DYNCREATE(CAUVDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CAUVDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
	float depth[MAX_COUNT_POINTS];
	float xlocation[MAX_COUNT_POINTS];
	float ylocation[MAX_COUNT_POINTS];
	float m_depth;
	float m_xlocation;
	float m_ylocation;
	int	  count;
	int   Curvexaxis;
	float sceneRot[3];
	int m_AlarmLevel;
	int m_Battery;
	bool m_BattroomStatef;
	bool m_BattroomStateb;
	bool m_DeviceroomState;
	bool m_ExtraroomState;
	bool m_AttituderoomState;
	char m_receDat[4];
	bool m_datalost;
	bool m_connetion;
	bool m_AlarmSound;
	unsigned int m_errCount;
	int m_origin;
};