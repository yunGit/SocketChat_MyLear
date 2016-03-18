#ifndef PACKET_H_
#define PACKET_H_

/*
	FILE : packet.h

	数据包处理函数的头
*/

namespace YUN
{
	//	在pPacket 参数的nPos位置放置 TYPE 类型的value的值
	//	可以 Put 的类型包括：BYTE,WORD,DWORD,int,short
	template<typename TYPE>
	void	PutValue(char* pPacket, TYPE value, int &nPos);

	//	把pPacket 参数nPos位置的值以 short 类型返回
	template<typename TYPE>
	TYPE	GetValue(char* pPacket, int &nPos);

	// 在pPacket 参数的nPos位置放置 字符串 类型的value的值
	void	PutString( char* pPacket, char* str, int &nPos );

	//	把位于 pPacket 参数 nPos位置的字符串数据复制到pBuffer(包括NULL的处理)
	void	GetString(char* pPacket, char* pBuffer, int &nPos);

	// 设置数据包的总长度
	void	PutSize(char* pPacket, WORD nPos);

}

#endif

// EOF