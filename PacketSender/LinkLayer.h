// LinkLayer.h: interface for the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"

class CLinkLayer 
: public CBaseLayer  
{
private:
	inline void	ResetHeader( );

public:
	u_short	tcp_port;
	void	StartThread();

	BOOL	Receive( u_char* ppayload ) ;
	BOOL	Send( u_char* ppayload, int nlength );

	void	SetEnetSrcAddress( u_char* pAddress );
	u_char*	GetEnetSrcAddress( );

	CLinkLayer( char* pName );
	virtual ~CLinkLayer();

protected:
	LINK_HEADER	m_sHeader ;
};

