// Joystick.cpp : implementation file
//

//	Name: Joystick.cpp
//	Function: class CCurveView
//	Date: Feb.8, 2011
//	Author: Tianli Zhou
#include "stdafx.h"
#include "Joystick.h"

int CJoystick::sw = 0;
CJoystick::CJoystick(int n)
{
	m_lpDIDevice = NULL;
	m_lpDI = NULL;
	m_hWnd = NULL;
	m_hInstance = GetModuleHandle(NULL) ; //获取实例句柄
	m_num = n;
}

CJoystick::~CJoystick(void)
{
	if(m_lpDIDevice)
		m_lpDIDevice->Unacquire();
		//释放DI和DIDevice对象
	if(m_lpDIDevice)
	{
		m_lpDIDevice->Release();
		m_lpDIDevice = NULL; 
	}
	if(m_lpDI)
	{
		m_lpDI->Release();
		m_lpDI = NULL; 
	}
}

bool CJoystick::Initialise(void)
{
	HRESULT hr;
	//建立DI8接口
	if(NULL == m_lpDI)
	{
		hr = DirectInput8Create(m_hInstance,
			DIRECTINPUT_VERSION, 
			IID_IDirectInput8,
			(void**)&m_lpDI, //接口取值
			NULL); 
		if FAILED(hr) 
		{
			OutputDebugString("Create 失败 - in CJoystick::Initialise\n");
			return false;
		}
	}
	hr = m_lpDI->EnumDevices( DI8DEVCLASS_GAMECTRL, 
		DIEnumDevicesCallback, //回调函数
		&JoystickGUID, //赋值GUID
		DIEDFL_ATTACHEDONLY ); //扫描安装好的和连接好的设备
	if FAILED(hr) 
	{
		OutputDebugString("枚举设备失败 - in CJoystick::Initialise\n");
		return false; 
	}
	//创建DI8设备
	if(!m_lpDIDevice)
	{
		hr = m_lpDI->CreateDevice(JoystickGUID,&m_lpDIDevice,NULL);
		if FAILED(hr) 
		{
			OutputDebugString("创建设备失败 - in CJoystick::Initialise\n");
			return false; 
		}
	}
	//设置协作等级—— 前台模式 | 独占模式
	/*hr = m_lpDIDevice ->SetCooperativeLevel(m_hWnd,DISCL_BACKGROUND);
	if FAILED(hr) 
	{
		OutputDebugString("设置协作等级失败 - in CJoystick::Initialise\n");
		//return false; 
	}*/
	//设置数据格式
	hr = m_lpDIDevice->SetDataFormat( &c_dfDIJoystick);
	if FAILED(hr) 
	{
		OutputDebugString("设置数据格式失败 - in CJoystick::Initialise\n");
		return false; 
	}
	hr = m_lpDIDevice->EnumObjects(EnumObjectsCallback, (VOID*)this, DIDFT_ALL );
	if FAILED(hr)
	{
		OutputDebugString("枚举对象失败 - in CJoystick::Initialise\n");
		return false; 
	}
	if(m_num == 0)
		sw = 1;
	else
		sw = 0;
	return TRUE;
}

BOOL CALLBACK CJoystick::DIEnumDevicesCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	*(GUID*) pvRef = lpddi->guidInstance;
	if(sw == 0)
		return DIENUM_CONTINUE;
	else
		return DIENUM_STOP; 
}

BOOL CALLBACK CJoystick::EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext )
{
	HRESULT hr;
	CJoystick * js = (CJoystick*)pContext; //首先取得JS对象指针
	//设置游戏杆输入特性
	if( pdidoi->dwType & DIDFT_AXIS ) //如果枚举的对象为轴
	{
		DIPROPRANGE diprg; //设置轴范围结构
		diprg.diph.dwSize = sizeof(DIPROPRANGE); 
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
		diprg.diph.dwHow = DIPH_BYID; 
		diprg.diph.dwObj = pdidoi->dwType; // 枚举的轴
		diprg.lMin = -5120; //最小值
		diprg.lMax = +5120; //最大值
		// 设置轴范围 
		hr = js->m_lpDIDevice->SetProperty( DIPROP_RANGE, &diprg.diph);
		if( FAILED(hr)) 
		{
			OutputDebugString("设置轴范围失败 - in CJoystick::EnumObjectsCallback\n");
			return DIENUM_STOP;
		}
		//设置死区属性，如果你使用的是电平式的游戏手柄，需要注释掉一下部分
		/*DIPROPDWORD dipdw; //死区结构
		dipdw.diph.dwSize = sizeof( dipdw ); 
		dipdw.diph.dwHeaderSize = sizeof( dipdw.diph ); 
		diprg.diph.dwObj = pdidoi->dwType; // 枚举的轴
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = 1000; //10%的死区
		hr = js->m_lpDIDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		if( FAILED(hr)) 
		{
			OutputDebugString("设置死区失败 - in CJoystick::EnumObjectsCallback\n");
			return DIENUM_STOP;
		}*/
	}
	return DIENUM_CONTINUE;
}

HRESULT CJoystick::PollDevice(void)
{
	HRESULT hr;
	if( NULL == m_lpDIDevice ) //未获得设备
		return S_OK;
	hr = m_lpDIDevice->Poll(); // 轮循设备读取当前状态
	if( FAILED(hr) ) 
	{
		// 输入流中断，不能通过轮循获得任何状态值。
		// 所以不需要任何重置，只要再次获得设备就行。
		hr = m_lpDIDevice->Acquire();
		while( hr == DIERR_INPUTLOST )
		{
			static int iCount = 0;
			if (iCount>30) exit(-1); //累积30次获取设备失败，退出程序。
			iCount++;
			OutputDebugString("丢失设备，轮循失败 - in CJoystick::PollDevice\n");
			hr = m_lpDIDevice->Acquire();
			if( SUCCEEDED(hr) ) iCount = 0; 
		} // hr也许为其他的错误.
	return S_OK; 
	}
	// 获得输入状态，存储到成员变量 m_diJs 中
	if( FAILED( hr = m_lpDIDevice->GetDeviceState( sizeof(DIJOYSTATE), &m_diJs ) ) )
		return hr; // 在轮循过程中设备将为 已获得 状态
	return S_OK;
}
