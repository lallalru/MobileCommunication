// NILayer.h: interface for the CNILayer class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"
#include <pcap.h>
#include <Packet32.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

class CNILayer 
	: public CBaseLayer
{
protected:
	pcap_t			*m_AdapterObject;

public:
	BOOL			m_thrdSwitch;
	u_char*  m_ppayload;

	void			PacketStartDriver();
	void			PacketStopDriver();

	pcap_if_t 		*GetAdapterObject(int iIndex);
	void			SetAdapterNumber(int iNum);
	void			SetAdapterList(LPADAPTER *plist);

	static UINT		ReadingThread(LPVOID pParam);
	static UINT		FileTransferThread(LPVOID pParam);

	BOOL			Receive( u_char *ppayload, int len ) ;
	BOOL			Send( u_char *ppayload, int nlength );

	CNILayer( char* pName, LPADAPTER *pAdapterObject = NULL, int iNumAdapter = 0 );
	virtual ~CNILayer();

	CString			GetNICardAddress(char* adapter_name);	// Adapter�̸����� Mac �ּ� ��ȯ
	LPIP_ADDR		GetIpAddress(u_char* ether);		// Mac �ּҷ� IP �ּ� ã�� ��ȯ
	u_char			*SubStr( const char *pnInput, int nLen );
protected:
	int			m_iNumAdapter;
	pcap_if_t	*m_pAdapterList[NI_COUNT_NIC];
};