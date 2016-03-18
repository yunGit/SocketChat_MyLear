#ifndef PACKET_H_
#define PACKET_H_

/*
	FILE : packet.h

	���ݰ���������ͷ
*/

namespace YUN
{
	//	��pPacket ������nPosλ�÷��� TYPE ���͵�value��ֵ
	//	���� Put �����Ͱ�����BYTE,WORD,DWORD,int,short
	template<typename TYPE>
	void	PutValue(char* pPacket, TYPE value, int &nPos);

	//	��pPacket ����nPosλ�õ�ֵ�� short ���ͷ���
	template<typename TYPE>
	TYPE	GetValue(char* pPacket, int &nPos);

	// ��pPacket ������nPosλ�÷��� �ַ��� ���͵�value��ֵ
	void	PutString( char* pPacket, char* str, int &nPos );

	//	��λ�� pPacket ���� nPosλ�õ��ַ������ݸ��Ƶ�pBuffer(����NULL�Ĵ���)
	void	GetString(char* pPacket, char* pBuffer, int &nPos);

	// �������ݰ����ܳ���
	void	PutSize(char* pPacket, WORD nPos);

}

#endif

// EOF