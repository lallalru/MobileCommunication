// SCTPLayer.h: interface for the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"

class CSCTPLayer 
	: public CBaseLayer  
{
private:
	inline void		ResetHeader( );
	u_char srcip[4];
	u_char destip[4];

	void SetChunkData(int nlength);

public:
	CSCTPLayer( char* pName );
	virtual ~CSCTPLayer();

	void SetSrcPortAddress(u_short src_port);
	void SetDstPortAddress(u_short dst_port);
	void SetFlags(char flags);
	void SetSSN(u_short ssn);

	BOOL Send(u_char* ppayload, int nlength);
	BOOL Receive(u_char* ppayload);

protected:
	SCTPLayer_HEADER	m_sHeader ;
	CHUNK_HEADER		m_sChunk ;
};