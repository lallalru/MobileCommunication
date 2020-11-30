// S1APLayer.h: interface for the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"
#include "SCTPLayer.h"

class CS1APLayer 
	: public CBaseLayer  
{
private:
	inline void		ResetHeader( );
	void	AttachReqMsg(int nlength);
	void	AuthenticationRepMsg(int nlength);
	void	initialContextSetupResponseItems(int nlength);
	void	attachCompleteItems(int nlength);

	u_char	SearchItems(u_char proc, u_char* ppayload);

	void	SelectMessage(int nlength);

public:
	int		MESSAGE_TYPE;

	CS1APLayer( char* pName );
	virtual ~CS1APLayer();
	
	BOOL Send(u_char* ppayload, int nlength);
	BOOL Receive(u_char* ppayload,int len);
	
	void SetMessageType(int msg_type);
	void SetTheNumberOfItems(char num);
	void SetTheNumberOfItems2(char num);
	
protected:
	S1APLayer_HEADER	m_sHeader ;
	S1APLayer_HEADER2	m_sHeader2 ;
};