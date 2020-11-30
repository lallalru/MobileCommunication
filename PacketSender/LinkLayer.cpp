// LinkLayer.cpp: implementation of the CLinkLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PacketSender.h"
#include "LinkLayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkLayer::CLinkLayer( char* pName )
: CBaseLayer( pName )
{
	ResetHeader( ) ;
}

CLinkLayer::~CLinkLayer()
{
}

void CLinkLayer::ResetHeader()
{
	/*m_sHeader.link_packet_type = ntohs(0x0004);
	m_sHeader.link_addr_type =  ntohs(0x0001);
	m_sHeader.link_addr_length =  ntohs(0x0006);*/
	m_sHeader.link_packet_type = ntohs(0x000e);
	m_sHeader.link_addr_type =  ntohs(0xdc62);
	m_sHeader.link_addr_length =  ntohs(0x1446);

	m_sHeader.link_proto = ntohs(ETHER_PROTO_TYPE_IP) ; // 0x0800
	memset(m_sHeader.link_data,'\0',ETHER_MAX_DATA_SIZE);
}

u_char* CLinkLayer::GetEnetSrcAddress() // Ethernet Source Address
{
	return m_sHeader.link_srcaddr.addrs;
}

void CLinkLayer::SetEnetSrcAddress(u_char *pAddress)
{
	// 이더넷 주소 12자리를 가져온다. (NI Layer에서 가져온다.)
	ETHERNET_ADDR src_ether;
	src_ether.addr0 = pAddress[0];
	src_ether.addr1 = pAddress[1];
	src_ether.addr2 = pAddress[2];
	src_ether.addr3 = pAddress[3];
	src_ether.addr4 = pAddress[4];
	src_ether.addr5 = pAddress[5];

	memcpy( m_sHeader.link_srcaddr.addrs, pAddress, 6 ) ;
}

BOOL CLinkLayer::Send(u_char *ppayload, int nlength)
{
	memcpy( m_sHeader.link_data , ppayload, nlength ) ;

	BOOL bSuccess = FALSE ;
	bSuccess = mp_UnderLayer->Send((u_char*) &m_sHeader,nlength + LINK_HEADER_SIZE);

	return bSuccess ;
}
	
BOOL CLinkLayer::Receive( u_char* ppayload )
{
	// 하위 계층에서 받은 payload를 현재 계층의 header구조에 맞게 읽음.
	PETHERNET_HEADER pFrame = (PETHERNET_HEADER) ppayload ;

	BOOL bSuccess = FALSE ;
	// 나에게 온 Ethernet Address가 아니면 discard 한다.
	// if (/* Fill the blank */ 0)
	// {
		bSuccess = mp_aUpperLayer[0]->Receive((u_char*)pFrame->enet_data);
	// }
	return bSuccess ;
}
