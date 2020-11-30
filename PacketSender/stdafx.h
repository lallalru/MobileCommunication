
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.

// Deligate Macro
#define DELG_SETTING						0
#define DELG_RESET							1
#define DELG_START							2
#define DELG_STOP							3
#define DELG_DEFAULT						4

// Timer Macro
#define TIMER_ENTRY_UP			5000	// 5초
#define TIMER(s)				s*1000	// 초
#define TIMER_ID_INIT			9999	// 중복 수신 체크
#define TIMER_STUD_ENTRY		10

// Global Macro
#define ERROR_INT				0xffff

// Protocol Macro
#define S1AP_PDU_INITIAL_MESSAGE			0x00
#define S1AP_PDU_SUCCESSFUL_OUTCOME			0x20

#define S1AP_ID_MME_UE_S1AP_ID				0
#define S1AP_ID_ENB_UE_S1AP_ID				8
#define S1AP_ID_E_RAB_TOBE_SETUP_LIST_CTXT_SUREQ	24
#define S1AP_ID_NAS_PDU						26
#define S1AP_ID_E_RABSETUP_ITEM_CTXT_SURES	50
#define S1AP_ID_E_RABSETUP_LIST_CTXT_SURES	51
#define S1AP_ID_E_RAB_TOBE_SETUP_ITEM_CTXT_SUREQ	52
#define S1AP_ID_TAI							67
#define S1AP_ID_EUTRAN_CGI					100
#define S1AP_ID_RRC_ESTABLISHMENT_CAUSE		134

#define S1AP_CRITICALITY_REJECT				0
#define S1AP_CRITICALITY_IGNORE				1

#define S1AP_PROC_CODE_INITIAL_CONTEXTSETUP	9
#define S1AP_PROC_CODE_DOWNLINK_NAS_TRANS	11
#define S1AP_PROC_CODE_INITIAL_UE_MSG		12
#define S1AP_PROC_CODE_UPLINK_NAS_TRANS		13

#define S1AP_MSG_TYPE_ATTACH_REQUEST		0x41
#define S1AP_MSG_TYPE_AUTHEN_REQUEST		0x52
#define S1AP_MSG_TYPE_AUTHEN_RESPONSE		0x53
#define S1AP_MSG_TYPE_ATTACH_ACCEPT			0x42
#define S1AP_MSG_TYPE_ATTACH_COMPLETE1		0xFF
#define S1AP_MSG_TYPE_ATTACH_COMPLETE2		0x43

#define	 SCTP_FLAGS_E						1
#define	 SCTP_FLAGS_B						2
#define	 SCTP_FLAGS_U						4
#define	 SCTP_FLAGS_I						8

#define MAX_LAYER_NUMBER		0xff
#define NI_COUNT_NIC			10
#define APP_STUDENT_NUM			9
#define IP_PROTO_SCTP			132

#define ETHER_MAX_SIZE			1514
#define ETHER_HEADER_SIZE		14
#define ETHER_MAX_DATA_SIZE		( ETHER_MAX_SIZE - ETHER_HEADER_SIZE )
#define ETHER_PROTO_TYPE_IP		0x0800
#define ETHER_PROTO_TYPE_ARP	0x0806
#define ETHER_PROTO_TYPE_RARP	0x0835

#define LINK_HEADER_SIZE		( sizeof(u_short) * 4 + \
									sizeof(u_char) * 6 )
#define LINK_MAX_DATA_SIZE		( ETHER_MAX_SIZE - LINK_HEADER_SIZE )

#define IP_HEADER_SIZE			( sizeof(u_short) * 4 +	\
									sizeof(u_char) * 12  )
#define IP_DATA_SIZE			( ETHER_MAX_SIZE - ( IP_HEADER_SIZE  + \
									LINK_HEADER_SIZE ) )

#define CHUNK_HEADER_SIZE		( sizeof(u_char) * 2 + sizeof(u_short) )
#define CHUNK_DATA_SIZE			( sizeof(u_char) * 12 )

#define SCTP_HEADER_SIZE		( sizeof(u_short) * 2 + \
									sizeof(u_char) * 8 + \
									CHUNK_HEADER_SIZE )
#define SCTP_DATA_SIZE			IP_DATA_SIZE - SCTP_HEADER_SIZE

#define S1AP_HEADER_SIZE		( sizeof(u_char) * (3+4) )
#define S1AP_DATA_SIZE			SCTP_DATA_SIZE - S1AP_HEADER_SIZE

#define ITEM_HEADER_SIZE		( sizeof(short) * 1 + sizeof(u_char) * 2 )
#define ITEM_DATA_SIZE			60

typedef struct _IP_ADDR
{
	union {
		struct { u_char i0, i1, i2, i3; } s_un_byte ;
		u_char s_ip_addr[4] ;
	} S_un ;

#define addr_i0 S_un.s_un_byte.i0
#define addr_i1 S_un.s_un_byte.i1
#define addr_i2 S_un.s_un_byte.i2
#define addr_i3 S_un.s_un_byte.i3
#define addrs_i S_un.s_ip_addr
} IP_ADDR, *LPIP_ADDR ;

typedef struct _ETHERNET_ADDR
{
	union {
		struct { u_char e0, e1, e2, e3, e4, e5; } s_un_byte ;
		u_char s_ether_addr[6] ;
	} S_un ;

#define addr0 S_un.s_un_byte.e0
#define addr1 S_un.s_un_byte.e1
#define addr2 S_un.s_un_byte.e2
#define addr3 S_un.s_un_byte.e3
#define addr4 S_un.s_un_byte.e4
#define addr5 S_un.s_un_byte.e5
#define addrs S_un.s_ether_addr

} ETHERNET_ADDR, *LPETHERNET_ADDR ;

// STUD Entry
typedef struct _STUD_ENTRY {
	IP_ADDR		ipaddr;
	u_int		studnum;
	u_int		timer;
} STUD_ENTRY, *PSTUD_ENTRY ;

// Array List
typedef struct _STUD_NODE{
	STUD_ENTRY	key;
}STUD_NODE, *PSTUD_NODE;

// Protocol struct define
typedef struct _ITEM_HEADER {
	u_short	id;
	u_char	criticality;
	u_char	length; // variable length
	u_char*	value;
} ITEM_HEADER, *PITEM_HEADER ;

typedef struct _S1APLayer_HEADER {
	u_char	s1ap_pdu; // a type of PDU
	u_char	s1ap_proc_code; // procedure code
	u_char	s1ap_crit; // criticality
	u_char	s1ap_length; // data length
	u_short	s1ap_padding; //TODO I don't know!!
	u_char	s1ap_nitem; // the number of items
	u_char	s1ap_data[S1AP_DATA_SIZE]; // variable length data
} S1APLayer_HEADER, *PS1APLayer_HEADER ;

typedef struct _S1APLayer_HEADER2 {
	u_char	s1ap_pdu; // a type of PDU
	u_char	s1ap_proc_code; // procedure code
	u_short	s1ap_crit; // criticality
	u_char	s1ap_length; // data length
	u_char	s1ap_padding[2]; //TODO I don't know!!
	u_char	s1ap_nitem; // the number of items
	u_char	s1ap_data[S1AP_DATA_SIZE]; // variable length data
} S1APLayer_HEADER2, *PS1APLayer_HEADER2 ;

typedef struct _CHUNK_HEADER {
	u_char	chunk_type;
	u_char	chunk_flags;
	u_short	chunk_length;
	u_int	chunk_tsn; // TSN
	u_short	chunk_sid; // Stream Id
	u_short	chunk_ssn; // Stream Sequence Number
	u_int	chunk_pid; // Payload protoccol Id
} CHUNK_HEADER, *PCHUNK_HEADER ;

typedef struct _SCTPLayer_HEADER {
	u_short	sctp_srcport;
	u_short	sctp_dstport;
	u_char	sctp_verif_tag[4];	// verification tag (no byteordering)
	u_char	sctp_checksum[4];
	CHUNK_HEADER	sctp_chunk;
	u_char	sctp_data[SCTP_DATA_SIZE]; // variable length data
} SCTPLayer_HEADER, *PSCTPLayer_HEADER ;

typedef struct _IPLayer_HEADER {
	u_char	ip_verlen;	// ip version		(1byte)
	u_char	ip_tos;		// type of service	(1byte)
	u_short	ip_len;		// total packet length	(2byte)
	u_short	ip_id;		// datagram id			(2byte)
	u_short	ip_fragoff;	// fragment offset		(2byte)
	u_char	ip_ttl;		// time to live in gateway hops (1byte)
	u_char	ip_proto;		// IP protocol			(1byte)
	u_short	ip_cksum;	// header checksum		(2byte)
	IP_ADDR			ip_src;	// IP address of source	(4byte)
	IP_ADDR			ip_dst;	// IP address of destiSTUDion	(4byte)
	u_char ip_data[IP_DATA_SIZE]; // variable length data

} IPLayer_HEADER, *PIPLayer_HEADER ;

typedef struct _LINK_HEADER {
	u_short	link_packet_type;
	u_short	link_addr_type;
	u_short	link_addr_length;
	ETHERNET_ADDR	link_srcaddr;		// source address of ethernet layer
	u_short	link_proto;			// type of ethernet layer
	u_char	link_data[ LINK_MAX_DATA_SIZE ] ; // frame data
} LINK_HEADER, *PLINK_HEADER ;

typedef struct _ETHERNET_HEADER {
	ETHERNET_ADDR	enet_dstaddr;		// destiSTUDion address of ethernet layer
	ETHERNET_ADDR	enet_srcaddr;		// source address of ethernet layer
	u_short	enet_type;			// type of ethernet layer
	u_char	enet_data[ ETHER_MAX_DATA_SIZE ] ; // frame data
} ETHERNET_HEADER, *PETHERNET_HEADER ;

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif