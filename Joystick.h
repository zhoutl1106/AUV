#pragma once

// Joystick.h : interface of the CJoystick class
//
//	Name: Joystick.h
//	Function: class CAUVView
//	Date: Feb.8, 2011
//	Author: Tianli Zhou

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class CJoystick
{
public:
	CJoystick(int n);
	virtual ~CJoystick();
	HINSTANCE m_hInstance; // 实例句柄
	HWND m_hWnd; // 窗口句柄
	LPDIRECTINPUT8 m_lpDI; // DI8接口指针
	LPDIRECTINPUTDEVICE8 m_lpDIDevice; // DIDevice8接口指针
	DIJOYSTATE m_diJs; //存储Joystick状态信息
	GUID JoystickGUID; //GUID
	//成员函数
	bool Initialise(void); //初始化函数
	//枚举设备
	static BOOL CALLBACK DIEnumDevicesCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef); //枚举对象
	static BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
	HRESULT PollDevice(void); // 轮循设备函数，在轮循过程中读取设备状态
	int m_num; //device serial number;
	static int sw;
};