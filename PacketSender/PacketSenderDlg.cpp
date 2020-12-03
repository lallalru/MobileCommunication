
// PacketSenderDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "PacketSender.h"
#include "PacketSenderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPacketSenderDlg ��ȭ ����
CPacketSenderDlg::CPacketSenderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPacketSenderDlg::IDD, pParent),
	CBaseLayer( "AppDlg" )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_LayerMgr.AddLayer( this ) ;	
	m_LayerMgr.AddLayer( new CNILayer( "NI" ) ) ;
	m_LayerMgr.AddLayer( new CLinkLayer( "Link" ) ) ;
	m_LayerMgr.AddLayer( new CIPLayer( "IP" ) );
	m_LayerMgr.AddLayer( new CSCTPLayer( "SCTP" ) );
	m_LayerMgr.AddLayer( new CS1APLayer( "S1AP" ) );

	// begin: �˸��� ���� ä��ÿ�
	m_LayerMgr.ConnectLayers(" NI ( *Link ( *IP ( *SCTP ( *S1AP ( *AppDlg ) ) ) ) ) ");
	// end

	m_IP = (CIPLayer *)m_LayerMgr.GetLayer("IP");
	m_Link = (CLinkLayer *)m_LayerMgr.GetLayer("Link");
	m_NI = (CNILayer *)m_LayerMgr.GetLayer("NI");
	m_SCTP = (CSCTPLayer *)m_LayerMgr.GetLayer("SCTP");
	m_S1AP = (CS1APLayer *)m_LayerMgr.GetLayer("S1AP");	
}

void CPacketSenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_SEND, btnSend);
	DDX_Control(pDX, IDC_SERVER_IPADDRESS, ctrlIP);
	DDX_Control(pDX,IDC_COMBO_MAC,comboMac);
}

BEGIN_MESSAGE_MAP(CPacketSenderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEND, &CPacketSenderDlg::OnBnClickedBtnSend)

	ON_BN_CLICKED(IDC_BTN_SETTING, &CPacketSenderDlg::OnBnClickedBtnSetting)
	ON_BN_CLICKED(IDC_BTN_START, &CPacketSenderDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CPacketSenderDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_RESET, &CPacketSenderDlg::OnBnClickedBtnReset)
	ON_CBN_SELCHANGE(IDC_COMBO_MAC, &CPacketSenderDlg::OnCbnSelchangeComboMac)
END_MESSAGE_MAP()


// CPacketSenderDlg �޽��� ó����

BOOL CPacketSenderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	int i;
	CString device_description;
	for(i=0;i<NI_COUNT_NIC;i++){
		if(!m_NI->GetAdapterObject(i))
			break;
		else{
			device_description = m_NI->GetAdapterObject(i)->description;
			device_description.Trim();
			comboMac.AddString(device_description);
		}
	}

	// �޺� �ڽ� �ʱⰪ ����
	comboMac.SetCurSel(0);
	CString NiName = m_NI->GetAdapterObject(0)->name;

	CString mac_address = m_NI->GetNICardAddress((char*)NiName.GetString());

	int indexMac = comboMac.GetCurSel();
	m_NI->SetAdapterNumber(indexMac);

	u_char ip[4];
	memcpy(ip,(*m_NI->GetIpAddress(MacAddrToHexInt(mac_address))).addrs_i,4);	

	ctrlIP.SetAddress(ip[0], ip[1], ip[2], 0); // Destination Address

	m_IP->SetSrcIPAddress(ip);
	m_Link->SetEnetSrcAddress(MacAddrToHexInt(mac_address));

	SetDeligate(DELG_DEFAULT);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CPacketSenderDlg::SetDeligate(int deligate)
{
	switch(deligate)
	{
	case DELG_SETTING :
		comboMac.EnableWindow(FALSE);
		(CIPAddressCtrl*)GetDlgItem(IDC_SERVER_IPADDRESS)->EnableWindow(FALSE);

		(CButton*)GetDlgItem(IDC_BTN_SETTING)->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BTN_RESET)->EnableWindow(TRUE);
		(CButton*)GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		(CButton*)GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
		break;

	case DELG_RESET:
		comboMac.EnableWindow(TRUE);
		(CIPAddressCtrl*)GetDlgItem(IDC_SERVER_IPADDRESS)->EnableWindow(TRUE);

		(CButton*)GetDlgItem(IDC_BTN_SETTING)->EnableWindow(TRUE);
		(CButton*)GetDlgItem(IDC_BTN_RESET)->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
		break;

	case DELG_START :
		comboMac.EnableWindow(FALSE);
		(CIPAddressCtrl*)GetDlgItem(IDC_SERVER_IPADDRESS)->EnableWindow(FALSE);

		(CButton*)GetDlgItem(IDC_BTN_SETTING)->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BTN_RESET)->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
		(CButton*)GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
		break;

	case DELG_STOP :
		comboMac.EnableWindow(FALSE);
		(CIPAddressCtrl*)GetDlgItem(IDC_SERVER_IPADDRESS)->EnableWindow(FALSE);

		(CButton*)GetDlgItem(IDC_BTN_SETTING)->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BTN_RESET)->EnableWindow(TRUE);
		(CButton*)GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		(CButton*)GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
		break;

	default:
		comboMac.EnableWindow(TRUE);
		(CIPAddressCtrl*)GetDlgItem(IDC_SERVER_IPADDRESS)->EnableWindow(TRUE);

		(CButton*)GetDlgItem(IDC_BTN_SETTING)->EnableWindow(TRUE);
		(CButton*)GetDlgItem(IDC_BTN_RESET)->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
		break;
	}
}

void CPacketSenderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CPacketSenderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CPacketSenderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CPacketSenderDlg::Send(u_char* ppayload, int length)
{
	BOOL bSuccess = FALSE;
	bSuccess = mp_UnderLayer->Send((u_char*)ppayload,length);

	return bSuccess;
}

BOOL CPacketSenderDlg::Receive(u_char* ppayload)
{
	BOOL bSuccess = FALSE;

	// � �޽��� ���� Ȯ���ϰ� �˸´� �޽��� ����!!!
	// ���� ���̾�κ��� ���� ������ ��������
	// �Ʒ� ���ǹ��� �̿��ؼ� Req/Rep �޽����� ó��!!!!
	u_char spayload[S1AP_DATA_SIZE];
	int length = 0;
	char items = 0;
	memset(spayload, '\0', S1AP_DATA_SIZE);

	// ���� �޽��� Ÿ�� Ȯ�� �� ó��
	switch (ppayload[0])
	{
		// UE
	case S1AP_MSG_TYPE_AUTHEN_REQUEST:// ���� �޽��� Ÿ�� (�� ��° ����)
		length = 57;

		// begin: �˸��� ���� ä��ÿ�
		memcpy(spayload, "\0", length); // ���� �޽��� ��Ŷ ������ (���� ���� �Ʒ� �Լ� ȣ��)
		m_S1AP->SetMessageType(0); // ���� �޽��� Ÿ��
		m_S1AP->SetTheNumberOfItems(0); // ���� �޽����� ������ ����
		// end
		break;

		// UE
	case S1AP_MSG_TYPE_ATTACH_ACCEPT: // ���� �޽��� Ÿ�� (�� ��° ����)
		length = 32;

		// begin: �˸��� ���� ä��ÿ�
		memcpy(spayload, "\0", length); // ���� �޽��� ��Ŷ ������ (���� ���� �Ʒ� �Լ� ȣ��)
		m_S1AP->SetMessageType(0); // ���� �޽��� Ÿ��
		m_S1AP->SetTheNumberOfItems(0); // ���� �޽����� ������ ����
		// end

		Send(spayload, length);

		// Attach Complete
		length = 53;

		// begin: �˸��� ���� ä��ÿ�
		memcpy(spayload, "\0", length); // ���� �޽��� ��Ŷ ������ (���� ���� �Ʒ� �Լ� ȣ��)
		m_S1AP->SetMessageType(0); // ���� �޽��� Ÿ��
		m_S1AP->SetTheNumberOfItems(0); // ���� �޽����� ������ ����
		// end
		break;
	default:
		return FALSE;
	}

	Send(spayload, length);
	return bSuccess;
}

u_char* CPacketSenderDlg::MacAddrToHexInt(CString ether)
{
	// �ݷ�(:)���� ���� �Ǿ��� Ethernet �ּҸ�
	// �ݷ�(:)�� ��ū���� �� ����Ʈ�� ���� �����ͼ� Ethernet�迭�� �־��ش�.
	CString cstr;
	u_char *arp_ether = (u_char *)malloc(sizeof(u_char)*6);

	for(int i=0;i<6;i++){
		AfxExtractSubString(cstr,ether,i,':');
		// strtoul -> ���ڿ��� ���ϴ� ������ ��ȯ �����ش�.
		arp_ether[i] = (u_char)strtoul(cstr.GetString(),NULL,16);
	}
	arp_ether[6] = '\0';

	return arp_ether;
}

void CPacketSenderDlg::OnBnClickedBtnSend()
{
	// �޽����� ���� Item�� ä���� ���������� ��.
	u_char ppayload[S1AP_DATA_SIZE];
	int length=0;
	char items=0;

	length = 90;
	memset(ppayload,'\0',S1AP_DATA_SIZE);
	memcpy(ppayload,attachReqItems(),length);

	items = 5;

	m_S1AP->SetTheNumberOfItems(items);
	m_S1AP->SetMessageType(S1AP_MSG_TYPE_ATTACH_REQUEST);
	Send(ppayload,length);
}

void CPacketSenderDlg::OnBnClickedBtnSetting()
{
	u_char dstip[4];
	// 168.188.127.166 a8.bc.7f.a6
	/*
	u_char dstip[4] = {
		0xa8,0xbc,0x7f,0xa6 
	};
	*/

	ctrlIP.GetAddress(dstip[0],dstip[1],dstip[2],dstip[3]);

	m_IP->SetDstIPAddress(dstip);

	// ������ NICard�� Adapter ���
	int indexMac = comboMac.GetCurSel();
	m_NI->SetAdapterNumber(indexMac);

	SetDeligate(DELG_SETTING);
}

void CPacketSenderDlg::OnBnClickedBtnReset()
{
	SetDeligate(DELG_RESET);
}

void CPacketSenderDlg::OnBnClickedBtnStart()
{
	// PacketDriver ����
	m_NI->PacketStartDriver();

	SetDeligate(DELG_START);
}

void CPacketSenderDlg::OnBnClickedBtnStop()
{
	// PacketDriver ����
	m_NI->PacketStopDriver();

	SetDeligate(DELG_STOP);
}

void CPacketSenderDlg::OnCbnSelchangeComboMac()
{
	UpdateData ( TRUE );

	// NICard �̸����� Mac address ��������

	// ComboBox���� ���õ� �ε��� ��ȣ ��������
	int indexMac = comboMac.GetCurSel();

	// ������ �ε��� ��ȣ�� NIC�� Adapter �̸� ��������
	CString NiName = m_NI->GetAdapterObject(indexMac)->name;

	// Adapter �̸����� Mac Address�� ��������
	CString mac_address = m_NI->GetNICardAddress((char *)NiName.GetString());

	// Mac Address�� IP�� ��������
	u_char ip[4];
	memcpy(ip,(*m_NI->GetIpAddress(MacAddrToHexInt(mac_address))).addrs_i,4);	

	// ȭ�鿡 IP ����
	ctrlIP.SetAddress(ip[0],ip[1],ip[2],0);

	// IP Layer�� Source IP �ּ� ����
	m_IP->SetSrcIPAddress(ip);

	// Link Layer�� Source mac �ּ� ����
	m_Link->SetEnetSrcAddress(MacAddrToHexInt(mac_address));

	UpdateData ( FALSE );
}

int CPacketSenderDlg::item_size(ITEM_HEADER item)
{
	return 2 + 1 + 1 + item.length;
}

u_char* CPacketSenderDlg::attachReqItems() // (ù��° ����)
{
	u_char temp[93] = {
		// item0 id-eNB-UE-S1AP-ID
		0x00,0x08,0x00,0x02,0x00,0x00,

		// Item1 id-NAS-PDU
		///////////////////////////// Fill in the blank. ////////////////////////////////////////
		//0x00,0x1a,0x00,0x35,0x34,0x17,0x7c,0x2a,0x01,0x33,0x06,0x07,0x00,0x51,0x0b,0xf6
		//,0x54,0xf0,0x60,0x80,0x01,0x01,0xdb,0x00,0x15,0x3b,0x02,0xe0,0xe0,0x00,0x14,0x02
		//,0x01,0xd0,0x31,0xd1,0x27,0x0d,0x80,0x00,0x0d,0x00,0x00,0x03,0x00,0x00,0x0c,0x00
		//,0x00,0x01,0x00,0x52,0x54,0xf0,0x60,0x01,0xf4,
		0x00,0x1a,0x00,0x12,0x11,0x17,0x38,0x6f,0x95,0x5b,0x08,0x07,0x53,0x08,0xaa,0x7a,
		0xdf,0x21,0x9c,0xa2,0x52,0x82
		/////////////////////////////////////////////////////////////////////////////////////////

		// Item2 id-TAI
		,0x00,0x43,0x00,0x06,0x00,0x54,0xf0,0x60,0x01,0xf4

		// Item3 id-EUTRAN-CGI
		,0x00,0x64,0x40,0x08,0x00,0x54,0xf0,0x60,0x00,0x00,0x10,0xe0

		// Item4 id-RRC-Establishment-Cause
		,0x00,0x86,0x40,0x01,0x30

		// padding
		,0x00,0x00,0x00
	};

	return temp;
}

u_char* CPacketSenderDlg::authenticationReqItems()
{
	u_char temp[57] = {
		0
	};
	return temp;
}

u_char* CPacketSenderDlg::initialContextSetupRequestAttachAcceptItems()
{
	u_char temp[229] = {
		0
	};

	return temp;
}

u_char* CPacketSenderDlg::authenticationRspItems()
{
	u_char temp[64] = {
		0
	};

	return temp;
}

u_char*	CPacketSenderDlg::initialContextSetupResponseItems()
{
	u_char temp[33] = {
		0
	};

	return temp;
}

u_char*	CPacketSenderDlg::attachCompleteItems()
{
	u_char temp[57] = {
		0
	};

	return temp;
}
 