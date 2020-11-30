// IPLayer.h: interface for the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"

class CIPLayer 
: public CBaseLayer  
{
private:
	inline void	ResetHeader( );
	u_char		srcip[4];
	u_char		destip[4];

public:
	CIPLayer( char* pName );
	virtual ~CIPLayer();
	void	SetSrcIPAddress(u_char* src_ip);
	void	SetDstIPAddress(u_char* dst_ip);
	void	SetFragOff(u_short fragoff);

	BOOL	Send(u_char* ppayload, int nlength);
	BOOL	Receive(u_char* ppayload);

protected:
	IPLayer_HEADER	m_sHeader ;
};