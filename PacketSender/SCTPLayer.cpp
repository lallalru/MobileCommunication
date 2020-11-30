// SCTPLayer.cpp: implementation of the CSCTPLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PacketSender.h"
#include "SCTPLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSCTPLayer::CSCTPLayer( char* pName )
	: CBaseLayer( pName )
{
	ResetHeader( );
}

CSCTPLayer::~CSCTPLayer()
{
}

void CSCTPLayer::ResetHeader()
{
	// begin: 알맞은 값을 채우시오
	m_sHeader.sctp_srcport = htons(0000);
	m_sHeader.sctp_dstport = htons(0000); // 목적 포트
	// end

	m_sHeader.sctp_verif_tag[0] = 0x00;
	m_sHeader.sctp_verif_tag[1] = 0x00;
	m_sHeader.sctp_verif_tag[2] = 0x00;
	m_sHeader.sctp_verif_tag[3] = 0x00;
	memset(m_sHeader.sctp_checksum,'\0',4);
	memset(m_sHeader.sctp_data,'\0',SCTP_DATA_SIZE);

	// CHUNK DATA
	m_sChunk.chunk_type = 0x00; // DATA (0), INIT (1)
	m_sChunk.chunk_length = 0x00;
	m_sChunk.chunk_flags = 0x03;
	m_sChunk.chunk_tsn = 0x00000000;
	m_sChunk.chunk_sid = 0x0000;
	m_sChunk.chunk_ssn = 0x0041;
	m_sChunk.chunk_pid = 0x00000012;
	m_sHeader.sctp_chunk = m_sChunk;
}

void CSCTPLayer::SetSrcPortAddress(u_short src_port)
{
	m_sHeader.sctp_srcport = htons(src_port);
}

void CSCTPLayer::SetDstPortAddress(u_short dst_port)
{
	m_sHeader.sctp_dstport = htons(dst_port);
}

void CSCTPLayer::SetFlags(char flags)
{
	m_sChunk.chunk_flags = flags;
}

void CSCTPLayer::SetSSN(u_short ssn)
{
	m_sChunk.chunk_ssn = htons(ssn);
}

void CSCTPLayer::SetChunkData(int nlength)
{
	// CHUNK DATA
	m_sChunk.chunk_type = 0x00; // DATA (0), INIT (1)

	// begin: 알맞은 값을 채우시오
	m_sChunk.chunk_length = ntohs(0);
	m_sChunk.chunk_pid = htonl(0x00000000);
	// end

	m_sChunk.chunk_sid = htons(0x0001);
	m_sHeader.sctp_chunk = m_sChunk;
}

BOOL CSCTPLayer::Send(u_char* ppayload, int nlength)
{
	SetChunkData(nlength);
	
	memset(m_sHeader.sctp_data,'\0',SCTP_DATA_SIZE);
	memcpy(m_sHeader.sctp_data , ppayload , nlength) ;

	int cLength = -1;
	// Length 조절
	switch(htons(m_sChunk.chunk_ssn))
	{
	case 2:
		cLength -=3;
		break;
	case 6:
		cLength -=2;
		break;
	case 7:
		cLength -=3;
		break;
	}

	// begin: 알맞은 값을 채우시오
	int packet_length = 0 + nlength + cLength; /* + [SCTP PACKET TOTAL SIZE] */
	// end

	BOOL bSuccess = FALSE ;
	bSuccess = mp_UnderLayer->Send((u_char*)&m_sHeader,packet_length);

	return bSuccess;
}

BOOL CSCTPLayer::Receive(u_char* ppayload)
{
	PSCTPLayer_HEADER pFrame = (PSCTPLayer_HEADER) ppayload ;
	CHUNK_HEADER pChunk = pFrame->sctp_chunk;

	BOOL bSuccess = FALSE ;
	
	bSuccess = mp_aUpperLayer[0]->Receive((u_char*)pFrame->sctp_data,ntohs(pChunk.chunk_ssn));

	return bSuccess ;
}
