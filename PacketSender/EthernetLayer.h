// EthernetLayer.h: interface for the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ETHERNETLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_)
#define AFX_ETHERNETLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"

class CEthernetLayer 
: public CBaseLayer  
{
private:
	inline void		ResetHeader( );

public:
	unsigned short	tcp_port;
	void			StartThread();

	BOOL			Receive( unsigned char* ppayload ) ;
	BOOL			Send( unsigned char* ppayload, int nlength );

	void			SetEnetDstAddress( unsigned char* pAddress );
	void			SetEnetSrcAddress( unsigned char* pAddress );
	unsigned char*	GetEnetDstAddress( );
	unsigned char*	GetEnetSrcAddress( );

	CEthernetLayer( char* pName );
	virtual ~CEthernetLayer();

protected:
	ETHERNET_HEADER	m_sHeader ;
};

#endif // !defined(AFX_ETHERNETLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_)
