
// PacketSender.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CPacketSenderApp:
// �� Ŭ������ ������ ���ؼ��� PacketSender.cpp�� �����Ͻʽÿ�.
//

class CPacketSenderApp : public CWinApp
{
public:
	CPacketSenderApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CPacketSenderApp theApp;