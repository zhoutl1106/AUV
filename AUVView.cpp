
// AUVView.cpp : CAUVView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "AUV.h"
#endif

#define COMMPORT 4

#include "AUVDoc.h"
#include "AUVView.h"
#include "joystick.h"
#include "OpenGLView.h"
#include "CurveView.h"
#include "LocationView.h"
#include "DriveView.h"
#include "ShowView.h"
#include "CameraView.h"
#include "SerialPort.h"
#include <Windows.h>
#include "TopView.h"
#include "LeftView.h"
#include "BottomView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAUVView

IMPLEMENT_DYNCREATE(CAUVView, CView)

BEGIN_MESSAGE_MAP(CAUVView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CAUVView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_COMM_RXCHAR, OnCommunication)
ON_WM_CHAR()
END_MESSAGE_MAP()

// CAUVView 构造/析构
CJoystick joystick1(0);
CJoystick joystick2(1);
CSerialPort port2;
extern char comm[12];


CAUVView::CAUVView()
{
	// TODO: 在此处添加构造代码

}

CAUVView::~CAUVView()
{
	port2.StopMonitoring();
	port2.ClosePort();
	joystick1.~CJoystick();
	joystick2.~CJoystick();
	port2.~CSerialPort();
}

BOOL CAUVView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CAUVView 绘制

void CAUVView::OnDraw(CDC* pDC)
{
	CAUVDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(0,0,rect.Width(),rect.Width(),RGB(255,255,255));

	CFont newFont;
//  	newFont.CreateFont(80,40,0,0,400,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_MODERN,_T("文鼎CS长宋体繁"));
	newFont.CreatePointFont(360,"超研泽中楷体繁");
	pDC->SelectObject(&newFont);
	if(m_show)
// 		pDC->TextOut(0,0,"哈尔滨工程大学");
	pDC->TextOut(0,0,"哈爾濱工程大學");
	newFont.DeleteObject();
}


// CAUVView 打印


void CAUVView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CAUVView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CAUVView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CAUVView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CAUVView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CAUVView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CAUVView 诊断

#ifdef _DEBUG
void CAUVView::AssertValid() const
{
	CView::AssertValid();
}

void CAUVView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAUVDoc* CAUVView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAUVDoc)));
	return (CAUVDoc*)m_pDocument;
}
#endif //_DEBUG


// CAUVView 消息处理程序


int CAUVView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	joystick1.m_hWnd = m_hWnd; //首先获得窗口句柄
	joystick2.m_hWnd = m_hWnd;
	if(!joystick1.Initialise()) //初始化
	{
		OutputDebugString("初始化游戏杆失败 - in CAUVView::OnCreate\n");
	}
	if(!joystick2.Initialise()) //初始化
	{
		OutputDebugString("初始化游戏杆失败 - in CAUVView::OnCreate\n");
	}
	SetTimer(1,100,NULL);
	SetTimer(2,100,NULL); 
	SetTimer(3,800,NULL);
	SetTimer(99,1000,NULL);
	port2.InitPort(this,COMMPORT,9600);
	port2.StartMonitoring();
	m_sentCount = 0;
	m_receiveCount = 0;
	m_show = 0;

	CAUVDoc* pDoc = GetDocument();
	return 0;
}


void CAUVView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CAUVDoc* pDoc = GetDocument();
	CMainFrame* pMain = (CMainFrame* )AfxGetApp()->m_pMainWnd;

	if(nIDEvent == 1)
	{
		pDoc->count++;
		if(pDoc->count == MAX_COUNT_POINTS)
			pDoc->count = 0;
		pDoc->depth[pDoc->count] = pDoc->m_depth;
		pDoc->xlocation[pDoc->count] = pDoc->m_xlocation;
		pDoc->ylocation[pDoc->count] = pDoc->m_ylocation;
		if(NULL != pMain->m_wndSplitter.GetPane(0,0))
			pMain->m_wndSplitter.GetPane(0,0)->Invalidate(FALSE);
		if(NULL != pMain->m_wndSplitter.GetPane(0,1))
			pMain->m_wndSplitter.GetPane(0,1)->Invalidate(FALSE);	
		if(NULL != pMain->m_wndSplitter.GetPane(0,2))
			pMain->m_wndSplitter.GetPane(0,2)->Invalidate(FALSE);
		if(NULL != pMain->m_wndSplitter.GetPane(1,0))
			pMain->m_wndSplitter.GetPane(1,0)->Invalidate(FALSE);
		if(NULL != pMain->m_wndSplitter.GetPane(1,1))
			pMain->m_wndSplitter.GetPane(1,1)->Invalidate(FALSE);
		if(NULL != pMain->m_wndSplitter.GetPane(1,2))
			pMain->m_wndSplitter.GetPane(1,2)->Invalidate(FALSE);
		if(NULL != pMain->m_wndSplitter.GetPane(2,0))
			pMain->m_wndSplitter.GetPane(2,0)->Invalidate(FALSE);
		if(NULL != pMain->m_wndSplitter.GetPane(2,1))
			pMain->m_wndSplitter.GetPane(2,1)->Invalidate(FALSE);
		if(NULL != pMain->m_wndSplitter.GetPane(2,2))
			pMain->m_wndSplitter.GetPane(2,2)->Invalidate(FALSE);
	}
	if(nIDEvent ==2)
	{
		ProcessJoystick();		
	}
	if(nIDEvent ==3)
	{
		int i;
		for(i = 2;i<11;i++)
			if(comm[i] == 0x00)
				comm[i] = 0x01;
		port2.WriteToPort(comm);
		OutputDebugString("Command Sent!\n");
	}
	if(nIDEvent == 99)
	{
		pDoc->m_datalost = 1;
		KillTimer(99);
	}

	CView::OnTimer(nIDEvent);
}


void CAUVView::ProcessJoystick(void)
{
	CAUVDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return ;
	CString str;
	//joystick1 processing
	if(FAILED( joystick1.PollDevice() ) ) //轮循
	{
		KillTimer( 3 ); 
		MessageBox(TEXT("读取设备状态错误") \
			TEXT("程序即将退出"), TEXT("DirectInput 示例"), 
			MB_ICONERROR | MB_OK );
	}
 	//if (joystick1.m_diJs.lX != 0 )
	{
		//OutputDebugString(" x \n");
		//str.Format("%d\n",joystick1.m_diJs.lX);
		comm[2] = -joystick1.m_diJs.lX*126/5120.;
	}
	if (joystick1.m_diJs.lY != 0 )
	{
		//OutputDebugString(" y \n");
		//str.Format("%d\n",joystick1.m_diJs.lY);
		comm[3] = -joystick1.m_diJs.lY*126/5120.;
	}
	if (joystick1.m_diJs.lZ != 0 )
	{
		//OutputDebugString(" z \n");
	}
	if (joystick1.m_diJs.lRx != 0 )
	{
		OutputDebugString(" lRx \n");
	}
	if (joystick1.m_diJs.lRy != 0 )
	{
		OutputDebugString(" lRy \n");
	}
	//if (joystick1.m_diJs.lRz != 0 )
	{
		

	}
	if(joystick1.m_diJs.rglSlider[0] != 0)//+- 5120
	{
	}
	comm[4] = 0;
	if(joystick1.m_diJs.rgdwPOV[0] == 0)// inverse hour every 45 degrees
	{
		comm[4] = joystick2.m_diJs.rglSlider[0]*63/5120.-63;
	}
	if(joystick1.m_diJs.rgdwPOV[0] == 4500)// inverse hour every 45 degrees
	{
		OutputDebugString("45\n");
		comm[4] = joystick2.m_diJs.rglSlider[0]*63/5120.-63;
	}
	if(joystick1.m_diJs.rgdwPOV[0] == 9000)// inverse hour every 45 degrees
	{
		OutputDebugString("90\n");
// 		pDoc->sceneRot[1] = int(pDoc->sceneRot[1]-2)%360;
// 		if(pDoc->sceneRot[1]>180)
// 			pDoc->sceneRot[1]=(int)pDoc->sceneRot[1]%180-180;
// 		if(pDoc->sceneRot[1]<-180)
// 			pDoc->sceneRot[1]=(int)pDoc->sceneRot[1]%180+180;
	}
	if(joystick1.m_diJs.rgdwPOV[0] == 13500)// inverse hour every 45 degrees
	{
		OutputDebugString("135\n");
		comm[4] = -joystick2.m_diJs.rglSlider[0]*63/5120.+63;
	}
	if(joystick1.m_diJs.rgdwPOV[0] == 18000)// inverse hour every 45 degrees
	{
		OutputDebugString("180\n");
		comm[4] = -joystick2.m_diJs.rglSlider[0]*63/5120.+63;	
	}
	if(joystick1.m_diJs.rgdwPOV[0] == 22500)// inverse hour every 45 degrees
	{
		OutputDebugString("225\n");
		comm[4] = -joystick2.m_diJs.rglSlider[0]*63/5120.+63;
	}
	if(joystick1.m_diJs.rgdwPOV[0] == 27000)// inverse hour every 45 degrees
	{
		OutputDebugString("270\n");
// 		pDoc->sceneRot[1] = int(pDoc->sceneRot[1]+2)%360;
// 		if(pDoc->sceneRot[1]>180)
// 			pDoc->sceneRot[1]=(int)pDoc->sceneRot[1]%180-180;
// 		if(pDoc->sceneRot[1]<-180)
// 			pDoc->sceneRot[1]=(int)pDoc->sceneRot[1]%180+180;
	}
	if(joystick1.m_diJs.rgdwPOV[0] == 31500)// inverse hour every 45 degrees
	{
		OutputDebugString("315\n");
		comm[4] = joystick2.m_diJs.rglSlider[0]*63/5120.-63;
	}
	for(int i = 0; i < 32 ; i++)
		if (joystick1.m_diJs.rgbButtons[i] & 0x80)
		{

			str.Format("Joy1按钮%d键按下\n",i);
			
			OutputDebugString(str);
			//char buf[1]={i+1};
			//m_Port2.WriteToPort(buf);
			if(11 == i)
				pDoc->m_AlarmSound = !pDoc->m_AlarmSound;
		}

	//joystick2 processing
		if(FAILED( joystick2.PollDevice() ) ) //轮循
		{
			KillTimer( 3 ); 
			MessageBox(TEXT("读取设备状态错误") \
				TEXT("程序即将退出"), TEXT("DirectInput 示例"), 
				MB_ICONERROR | MB_OK );
		}
		if (joystick2.m_diJs.lX != 0 )
		{
			//OutputDebugString(" x \n");
		}
		if (joystick2.m_diJs.lY != 0 )
		{
			//OutputDebugString(" y \n");
		}
		if (joystick2.m_diJs.lZ != 0 )
		{
			//OutputDebugString(" z \n");
		}
		if (joystick2.m_diJs.lRx != 0 )
		{
			//OutputDebugString(" lRx \n");
		}
		if (joystick2.m_diJs.lRy != 0 )
		{
			//OutputDebugString(" lRy \n");
		}
		if (joystick2.m_diJs.lRz != 0 )
		{
			//char buf[1]={0x13};
		}
		if(joystick2.m_diJs.rglSlider[0] != 0)//+- 5120
		{
			//StringCchPrintf(ch,20,"%ld,%ld\n",joystick2.m_diJs.rglSlider[1],joystick2.m_diJs.rglSlider[0]);
		}
		if(joystick2.m_diJs.rgdwPOV[0] == 0)// inverse hour every 45 degrees
		{
			OutputDebugString("0\n");
		}
		if(joystick2.m_diJs.rgdwPOV[0] == 4500)// inverse hour every 45 degrees
		{
			OutputDebugString("45\n");
		}
		if(joystick2.m_diJs.rgdwPOV[0] == 9000)// inverse hour every 45 degrees
		{
			OutputDebugString("90\n");
		}
		if(joystick2.m_diJs.rgdwPOV[0] == 13500)// inverse hour every 45 degrees
		{
			OutputDebugString("135\n");
		}
		if(joystick2.m_diJs.rgdwPOV[0] == 18000)// inverse hour every 45 degrees
		{
			OutputDebugString("180\n");
		}
		if(joystick2.m_diJs.rgdwPOV[0] == 22500)// inverse hour every 45 degrees
		{
			OutputDebugString("225\n");
		}
		if(joystick2.m_diJs.rgdwPOV[0] == 27000)// inverse hour every 45 degrees
		{
			OutputDebugString("270\n");
		}
		if(joystick2.m_diJs.rgdwPOV[0] == 31500)// inverse hour every 45 degrees
		{
			OutputDebugString("315\n");
		}
		for(int i = 0; i < 32 ; i++)
			if (joystick2.m_diJs.rgbButtons[i] & 0x80)
			{
				str.Format("Joy2按钮%d键按下\n",i);
				OutputDebugString(str);
			}
}
LONG CAUVView::OnCommunication(WPARAM ch, LPARAM port)
{
	CAUVDoc *pDoc = GetDocument();
	CString str;
	static unsigned char cha[24];
	static unsigned char chamodify[14];
	cha[m_receiveCount] = (unsigned char)(ch);
	m_receiveCount++;
	KillTimer(99);
	SetTimer(99,1000,NULL);
	if(1 == m_receiveCount)
		if(0xaa != cha[0])
			m_receiveCount = 0;
	if(2 == m_receiveCount)
		if(0x55 != cha[1])
			m_receiveCount = 0;
	if(24 == m_receiveCount)
	{
		pDoc->m_datalost = 0;
		m_receiveCount = 0;
		str.Format("%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n",cha[0],cha[1],cha[2],cha[3],cha[4],cha[5],cha[6],cha[7],cha[8],cha[9],cha[10],cha[11],cha[12],cha[13],cha[14],cha[15],cha[16],cha[17],cha[18],cha[19],cha[20],cha[21],cha[22],cha[23]);
		OutputDebugString(str);
		if(0xaa == cha[0] && 0x55 == cha[1])
		{
			int count;
			chamodify[0] = cha[0];
			chamodify[1] = cha[1];
			chamodify[2] = cha[2];
			chamodify[13] = cha[23];

			for(count = 0;count<5;count++)
			{
				if(cha[4*count+3] == 0xa5 && cha[4*count+5] == 0x5a)
					chamodify[2*count+3] = 0x00;
				else
					chamodify[2*count+3] = cha[4*count+3];

				if(cha[4*count+4] == 0xa5 && cha[4*count+6] == 0x5a)
					chamodify[2*count+4] = 0x00;
				else
					chamodify[2*count+4] = cha[4*count+4];
			}

			str.Format("%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n",chamodify[0],chamodify[1],chamodify[2],chamodify[3],chamodify[4],chamodify[5],chamodify[6],chamodify[7],chamodify[8],chamodify[9],chamodify[10],chamodify[11],chamodify[12],chamodify[13]);
			OutputDebugString(str);

			//leak
			if(chamodify[2] & 0x80)
				pDoc->m_BattroomStatef = 0;
			else
				pDoc->m_BattroomStatef = 1;

			if(chamodify[2] & 0x40)
				pDoc->m_BattroomStateb = 0;
			else
				pDoc->m_BattroomStateb = 1;

			//depth
			if(chamodify[3]*100 + chamodify[4] < 9999)
				pDoc->m_depth = -(chamodify[3]*100+chamodify[4])/100.;
			//roll
			if(chamodify[5]*256 + chamodify[6] >10000)
				pDoc->sceneRot[2] = (chamodify[5]*256 + chamodify[6]-65535)/100.;
			else
				pDoc->sceneRot[2] = (chamodify[5]*256 + chamodify[6])/100.;
			//pitch
			if(chamodify[7]*256 + chamodify[8] >10000)
				pDoc->sceneRot[0] = (chamodify[7]*256 + chamodify[8]-65535)/100.;
			else
				pDoc->sceneRot[0] = (chamodify[7]*256 + chamodify[8])/100.;
			//yaw
			if(chamodify[9]*256 + chamodify[10] >10000)
				pDoc->sceneRot[1] = (chamodify[9]*256 + chamodify[10]-65535)/100.;
			else
				pDoc->sceneRot[1] = (chamodify[9]*256 + chamodify[10])/100.;
			//battery
			if(!(chamodify[11]&0xf0))
				pDoc->m_Battery = chamodify[11]*256+chamodify[12]; //0.01V
			//lost
			if(chamodify[13] == 0xf5)
				pDoc->m_connetion = 0;
			else if(chamodify[13] == 0x5a)
				pDoc->m_connetion = 1;
			else
				pDoc->m_datalost = 1;
		}
	}
	return 0;
}

void CAUVView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CAUVDoc *pDoc = GetDocument();
	if('t' == nChar)
		m_show = !m_show;
	if('m' == nChar)
		pDoc->m_AlarmSound = !pDoc->m_AlarmSound;
	Invalidate(FALSE);

	CView::OnChar(nChar, nRepCnt, nFlags);
}