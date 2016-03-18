/*
	FILE : packet.cpp

	数据包处理函数
*/

#include "Def.h"
#include "Packet.h"

namespace YUN
{
	//	在pPacket 参数的nPos位置放置 TYPE 类型的value的值
	template<typename TYPE>
	void	PutValue(char* pPacket, TYPE value, int &nPos)
	{
		*(TYPE*)(pPacket + nPos) = value;
		nPos += sizeof(TYPE);
	}

	//	把pPacket 参数nPos位置的值以 short 类型返回
	template<typename TYPE>
	TYPE	GetValue(char* pPacket, int &nPos)
	{
		TYPE value = *(TYPE*)(pPacket + nPos);
		nPos += sizeof(TYPE);
		return value;
	}

	// 在pPacket 参数的nPos位置放置 字符串 类型的value的值
	void	PutString( char* pPacket, char* str, int &nPos )
	{
		*(WORD*)(pPacket + nPos) = strlen(str);
		nPos += sizeof(DWORD);
		memcpy(pPacket + nPos, str, strlen(str));
		nPos += strlen(str);
	}

	//	把位于 pPacket 参数 nPos位置的字符串数据复制到pBuffer(包括NULL的处理)
	void	GetString(char* pPacket, char* pBuffer, int &nPos)
	{
		WORD BufferLen = *(WORD*)(pPacket + nPos);
		nPos += nPos;
		memcpy(pPacket + nPos, pBuffer, BufferLen);
		*(pBuffer + BufferLen) = '\0';
		nPos += BufferLen;
	}
	// 设置数据包的总长度
	void	PutSize(char* pPacket, WORD nPos)
	{
		*(WORD*)(pPacket + nPos) = nPos;
	}
}

// EOF