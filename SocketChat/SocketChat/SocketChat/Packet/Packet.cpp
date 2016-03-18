/*
	FILE : packet.cpp

	���ݰ�������
*/

#include "Def.h"
#include "Packet.h"

namespace YUN
{
	//	��pPacket ������nPosλ�÷��� TYPE ���͵�value��ֵ
	template<typename TYPE>
	void	PutValue(char* pPacket, TYPE value, int &nPos)
	{
		*(TYPE*)(pPacket + nPos) = value;
		nPos += sizeof(TYPE);
	}

	//	��pPacket ����nPosλ�õ�ֵ�� short ���ͷ���
	template<typename TYPE>
	TYPE	GetValue(char* pPacket, int &nPos)
	{
		TYPE value = *(TYPE*)(pPacket + nPos);
		nPos += sizeof(TYPE);
		return value;
	}

	// ��pPacket ������nPosλ�÷��� �ַ��� ���͵�value��ֵ
	void	PutString( char* pPacket, char* str, int &nPos )
	{
		*(WORD*)(pPacket + nPos) = strlen(str);
		nPos += sizeof(DWORD);
		memcpy(pPacket + nPos, str, strlen(str));
		nPos += strlen(str);
	}

	//	��λ�� pPacket ���� nPosλ�õ��ַ������ݸ��Ƶ�pBuffer(����NULL�Ĵ���)
	void	GetString(char* pPacket, char* pBuffer, int &nPos)
	{
		WORD BufferLen = *(WORD*)(pPacket + nPos);
		nPos += nPos;
		memcpy(pPacket + nPos, pBuffer, BufferLen);
		*(pBuffer + BufferLen) = '\0';
		nPos += BufferLen;
	}
	// �������ݰ����ܳ���
	void	PutSize(char* pPacket, WORD nPos)
	{
		*(WORD*)(pPacket + nPos) = nPos;
	}
}

// EOF