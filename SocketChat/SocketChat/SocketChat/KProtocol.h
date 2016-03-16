#ifndef KPROTOCOL_H_
#define KPROTOCOL_H_

#include "Platform.h"
#include "KProtocolDef.h"

namespace YUN
{
	// 2 level protocol struct
	struct CommonChildProtocolHead
	{
		BYTE	ProtocolType;
		WORD	ProtocolLen;
		BYTE	ChildProtocolType;
	};


}

#endif