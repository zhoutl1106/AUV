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
	m_hInstance = GetModuleHandle(NULL) ; //��ȡʵ�����
	m_num = n;
}

CJoystick::~CJoystick(void)
{
	if(m_lpDIDevice)
		m_lpDIDevice->Unacquire();
		//�ͷ�DI��DIDevice����
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
	//����DI8�ӿ�
	if(NULL == m_lpDI)
	{
		hr = DirectInput8Create(m_hInstance,
			DIRECTINPUT_VERSION, 
			IID_IDirectInput8,
			(void**)&m_lpDI, //�ӿ�ȡֵ
			NULL); 
		if FAILED(hr) 
		{
			OutputDebugString("Create ʧ�� - in CJoystick::Initialise\n");
			return false;
		}
	}
	hr = m_lpDI->EnumDevices( DI8DEVCLASS_GAMECTRL, 
		DIEnumDevicesCallback, //�ص�����
		&JoystickGUID, //��ֵGUID
		DIEDFL_ATTACHEDONLY ); //ɨ�谲װ�õĺ����Ӻõ��豸
	if FAILED(hr) 
	{
		OutputDebugString("ö���豸ʧ�� - in CJoystick::Initialise\n");
		return false; 
	}
	//����DI8�豸
	if(!m_lpDIDevice)
	{
		hr = m_lpDI->CreateDevice(JoystickGUID,&m_lpDIDevice,NULL);
		if FAILED(hr) 
		{
			OutputDebugString("�����豸ʧ�� - in CJoystick::Initialise\n");
			return false; 
		}
	}
	//����Э���ȼ����� ǰ̨ģʽ | ��ռģʽ
	/*hr = m_lpDIDevice ->SetCooperativeLevel(m_hWnd,DISCL_BACKGROUND);
	if FAILED(hr) 
	{
		OutputDebugString("����Э���ȼ�ʧ�� - in CJoystick::Initialise\n");
		//return false; 
	}*/
	//�������ݸ�ʽ
	hr = m_lpDIDevice->SetDataFormat( &c_dfDIJoystick);
	if FAILED(hr) 
	{
		OutputDebugString("�������ݸ�ʽʧ�� - in CJoystick::Initialise\n");
		return false; 
	}
	hr = m_lpDIDevice->EnumObjects(EnumObjectsCallback, (VOID*)this, DIDFT_ALL );
	if FAILED(hr)
	{
		OutputDebugString("ö�ٶ���ʧ�� - in CJoystick::Initialise\n");
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
	CJoystick * js = (CJoystick*)pContext; //����ȡ��JS����ָ��
	//������Ϸ����������
	if( pdidoi->dwType & DIDFT_AXIS ) //���ö�ٵĶ���Ϊ��
	{
		DIPROPRANGE diprg; //�����᷶Χ�ṹ
		diprg.diph.dwSize = sizeof(DIPROPRANGE); 
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
		diprg.diph.dwHow = DIPH_BYID; 
		diprg.diph.dwObj = pdidoi->dwType; // ö�ٵ���
		diprg.lMin = -5120; //��Сֵ
		diprg.lMax = +5120; //���ֵ
		// �����᷶Χ 
		hr = js->m_lpDIDevice->SetProperty( DIPROP_RANGE, &diprg.diph);
		if( FAILED(hr)) 
		{
			OutputDebugString("�����᷶Χʧ�� - in CJoystick::EnumObjectsCallback\n");
			return DIENUM_STOP;
		}
		//�����������ԣ������ʹ�õ��ǵ�ƽʽ����Ϸ�ֱ�����Ҫע�͵�һ�²���
		/*DIPROPDWORD dipdw; //�����ṹ
		dipdw.diph.dwSize = sizeof( dipdw ); 
		dipdw.diph.dwHeaderSize = sizeof( dipdw.diph ); 
		diprg.diph.dwObj = pdidoi->dwType; // ö�ٵ���
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = 1000; //10%������
		hr = js->m_lpDIDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		if( FAILED(hr)) 
		{
			OutputDebugString("��������ʧ�� - in CJoystick::EnumObjectsCallback\n");
			return DIENUM_STOP;
		}*/
	}
	return DIENUM_CONTINUE;
}

HRESULT CJoystick::PollDevice(void)
{
	HRESULT hr;
	if( NULL == m_lpDIDevice ) //δ����豸
		return S_OK;
	hr = m_lpDIDevice->Poll(); // ��ѭ�豸��ȡ��ǰ״̬
	if( FAILED(hr) ) 
	{
		// �������жϣ�����ͨ����ѭ����κ�״ֵ̬��
		// ���Բ���Ҫ�κ����ã�ֻҪ�ٴλ���豸���С�
		hr = m_lpDIDevice->Acquire();
		while( hr == DIERR_INPUTLOST )
		{
			static int iCount = 0;
			if (iCount>30) exit(-1); //�ۻ�30�λ�ȡ�豸ʧ�ܣ��˳�����
			iCount++;
			OutputDebugString("��ʧ�豸����ѭʧ�� - in CJoystick::PollDevice\n");
			hr = m_lpDIDevice->Acquire();
			if( SUCCEEDED(hr) ) iCount = 0; 
		} // hrҲ��Ϊ�����Ĵ���.
	return S_OK; 
	}
	// �������״̬���洢����Ա���� m_diJs ��
	if( FAILED( hr = m_lpDIDevice->GetDeviceState( sizeof(DIJOYSTATE), &m_diJs ) ) )
		return hr; // ����ѭ�������豸��Ϊ �ѻ�� ״̬
	return S_OK;
}
