
// PacketSenderDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "PacketSender.h"
#include "PacketSenderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
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


// CPacketSenderDlg 대화 상자
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

	// begin: 알맞은 값을 채우시오
	m_LayerMgr.ConnectLayers("AppDlg");
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


// CPacketSenderDlg 메시지 처리기

BOOL CPacketSenderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
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

	// 콤보 박스 초기값 설정
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

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CPacketSenderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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

	// 어떤 메시지 인지 확인하고 알맞는 메시지 전송!!!
	// 하위 레이어로부터 받은 정보를 바탕으로
	// 아래 조건문을 이용해서 Req/Rep 메시지를 처리!!!!
	u_char spayload[S1AP_DATA_SIZE];
	int length = 0;
	char items = 0;
	memset(spayload, '\0', S1AP_DATA_SIZE);

	// 수신 메시지 타입 확인 후 처리
	switch (ppayload[0])
	{
		// UE
	case S1AP_MSG_TYPE_AUTHEN_REQUEST:// 받은 메시지 타입 (두 번째 과제)
		length = 57;

		// begin: 알맞은 값을 채우시오
		memcpy(spayload, "\0", length); // 보낼 메시지 패킷 데이터 (현재 파일 아래 함수 호출)
		m_S1AP->SetMessageType(0); // 보낼 메시지 타입
		m_S1AP->SetTheNumberOfItems(0); // 보낼 메시지의 아이템 개수
		// end
		break;

		// UE
	case S1AP_MSG_TYPE_ATTACH_ACCEPT: // 받은 메시지 타입 (세 번째 과제)
		length = 32;

		// begin: 알맞은 값을 채우시오
		memcpy(spayload, "\0", length); // 보낼 메시지 패킷 데이터 (현재 파일 아래 함수 호출)
		m_S1AP->SetMessageType(0); // 보낼 메시지 타입
		m_S1AP->SetTheNumberOfItems(0); // 보낼 메시지의 아이템 개수
		// end

		Send(spayload, length);

		// Attach Complete
		length = 53;

		// begin: 알맞은 값을 채우시오
		memcpy(spayload, "\0", length); // 보낼 메시지 패킷 데이터 (현재 파일 아래 함수 호출)
		m_S1AP->SetMessageType(0); // 보낼 메시지 타입
		m_S1AP->SetTheNumberOfItems(0); // 보낼 메시지의 아이템 개수
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
	// 콜론(:)으로 구분 되어진 Ethernet 주소를
	// 콜론(:)을 토큰으로 한 바이트씩 값을 가져와서 Ethernet배열에 넣어준다.
	CString cstr;
	u_char *arp_ether = (u_char *)malloc(sizeof(u_char)*6);

	for(int i=0;i<6;i++){
		AfxExtractSubString(cstr,ether,i,':');
		// strtoul -> 문자열을 원하는 진수로 변환 시켜준다.
		arp_ether[i] = (u_char)strtoul(cstr.GetString(),NULL,16);
	}
	arp_ether[6] = '\0';

	return arp_ether;
}

void CPacketSenderDlg::OnBnClickedBtnSend()
{
	// 메시지에 따라서 Item을 채워서 내려보내면 됨.
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

	// 선택한 NICard로 Adapter 등록
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
	// PacketDriver 실행
	m_NI->PacketStartDriver();

	SetDeligate(DELG_START);
}

void CPacketSenderDlg::OnBnClickedBtnStop()
{
	// PacketDriver 중지
	m_NI->PacketStopDriver();

	SetDeligate(DELG_STOP);
}

void CPacketSenderDlg::OnCbnSelchangeComboMac()
{
	UpdateData ( TRUE );

	// NICard 이름으로 Mac address 가져오기

	// ComboBox에서 선택된 인덱스 번호 가져오기
	int indexMac = comboMac.GetCurSel();

	// 가져온 인덱스 번호로 NIC의 Adapter 이름 가져오기
	CString NiName = m_NI->GetAdapterObject(indexMac)->name;

	// Adapter 이름으로 Mac Address를 가져오기
	CString mac_address = m_NI->GetNICardAddress((char *)NiName.GetString());

	// Mac Address로 IP를 가져오기
	u_char ip[4];
	memcpy(ip,(*m_NI->GetIpAddress(MacAddrToHexInt(mac_address))).addrs_i,4);	

	// 화면에 IP 설정
	ctrlIP.SetAddress(ip[0],ip[1],ip[2],0);

	// IP Layer에 Source IP 주소 설정
	m_IP->SetSrcIPAddress(ip);

	// Link Layer에 Source mac 주소 설정
	m_Link->SetEnetSrcAddress(MacAddrToHexInt(mac_address));

	UpdateData ( FALSE );
}

int CPacketSenderDlg::item_size(ITEM_HEADER item)
{
	return 2 + 1 + 1 + item.length;
}

u_char* CPacketSenderDlg::attachReqItems() // (첫번째 과제)
{
	u_char temp[93] = {
		// item0 id-eNB-UE-S1AP-ID
		0x00,0x08,0x00,0x02,0x00,0x00,

		// Item1 id-NAS-PDU
		///////////////////////////// Fill in the blank. ////////////////////////////////////////
		0x00,0x1a,0x00,0x35,0x34,0x17,0x7c,0x2a,0x01,0x33,0x06,0x07,0x00,0x51,0x0b,0xf6
		,0x54,0xf0,0x60,0x80,0x01,0x01,0xdb,0x00,0x15,0x3b,0x02,0xe0,0xe0,0x00,0x14,0x02
		,0x01,0xd0,0x31,0xd1,0x27,0x0d,0x80,0x00,0x0d,0x00,0x00,0x03,0x00,0x00,0x0c,0x00
		,0x00,0x01,0x00,0x52,0x54,0xf0,0x60,0x01,0xf4
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
 