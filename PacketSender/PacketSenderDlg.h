
// PacketSenderDlg.h : ��� ����
//
#pragma once
#include "LayerManager.h"	// Added by ClassView
#include "NILayer.h"		// Added by ClassView
#include "LinkLayer.h"		// Added by ClassView
#include "EthernetLayer.h"	// Added by ClassView
#include "IPLayer.h"		// Added by ClassView
#include "SCTPLayer.h"		// Added by ClassView
#include "S1APLayer.h"		// Added by ClassView

#include <iostream>
#include <string>
using namespace std;

// CPacketSenderDlg ��ȭ ����
class CPacketSenderDlg : public CDialogEx, public CBaseLayer
{
	// �����Դϴ�.
public:
	CPacketSenderDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PACKETSENDER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

	// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	CLayerManager	m_LayerMgr;
	CNILayer*		m_NI;
	CEthernetLayer*	m_Ethernet;
	CLinkLayer*		m_Link;
	CIPLayer*		m_IP;
	CSCTPLayer*		m_SCTP;
	CS1APLayer*		m_S1AP;

	CComboBox		comboMac;
	CIPAddressCtrl	ctrlIP;
	CButton			btnSetting;
	CButton			btnReset;
	CButton			btnStart;
	CButton			btnStop;
	CButton			btnSend;

public:
	BOOL Send(u_char* , int );
	BOOL Receive(u_char* );

	void		SetDeligate(int deligate);
	u_char*		MacAddrToHexInt(CString ether);
	int			item_size(ITEM_HEADER item);

	u_char*		attachReqItems();

	u_char*		identityReqItems(); // Acqusition of IMSI
	u_char*		identityRspItems();

	u_char*		authenticationReqItems(); // Authentication
	u_char*		authenticationRspItems();

	u_char*		initialContextSetupRequestAttachAcceptItems();
	u_char*		initialContextSetupResponseItems();
	u_char*		attachCompleteItems();

public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnIpnFieldchangedServerIpaddress(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboMac();
	afx_msg void OnBnClickedBtnSetting();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnSend();
};
