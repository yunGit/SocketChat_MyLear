/*
	FILE : message.cpp

	������յ����ݰ�����
*/

#include "..\Def.h"
#include "..\structs.h"
#include "..\Network\network.h"
#include "..\Packet\Packet.h"
#include "..\linked_list.h"
#include "..\Utils\utils.h"
#include "..\Message\message.h"

namespace YUN
{
	// ά������
	void PACKET_AliveCheck(sPCLIENT_DATA pClient)
	{

	}

	// �޸Ĵº�
	void PACKET_ChangeNick(sPCLIENT_DATA pClient)
	{
		char nick[64];
		GetString(pClient->m_recvBuff, nick, pClient->m_recvPos);

		strcpy(pClient->m_Name, nick);
	}

	// ������Ϣ����
	void  PACKET_RecvMsg(sPCLIENT_DATA pClient)
	{
		char msg[dMAX_SOCK_BUFF];

		GetString(pClient->m_recvBuff, msg, pClient->m_recvPos);

		int packetPos;
		char packet[dMAX_SOCK_BUFF];
		char tmp[dMAX_SOCK_BUFF];

		sprintf(tmp, "%s : %s\r\n", pClient->m_Name, msg);

		packetPos = 2;
		PutValue(packet, dPACKET_SEND_MSG, packetPos);
		PutString(packet, tmp, packetPos);
		PutSize(packet, packetPos);

		SendToAll(packet, packetPos);
	}

	// ���ݴ���
	BOOL ReadRecvBuff(sPCLIENT_DATA pClient)
	{
		WORD msgSize;
		WORD tag;

		while(pClient->m_recvSize > 0)
		{
			pClient->m_recvPos = 0;

			msgSize = GetValue<WORD>(pClient->m_recvBuff, pClient->m_recvPos);
			tag = GetValue<WORD>(pClient->m_recvBuff, pClient->m_recvPos);

			// ��Ŀǰ m_recvBuff �����ݵĳ���С�����ݰ��ܳ���ʱ
			if (pClient->m_recvSize < msgSize)
				return -1;

			// ���ݰ�����
			switch(tag)
			{
			case dPACKET_ALIVE_CHECK:
				PACKET_AliveCheck(pClient);
				break;
			case dPACKET_CHANGE_NICK:
				PACKET_ChangeNick(pClient);
				break;
			case dPACKET_RECV_MSG:
				PACKET_RecvMsg(pClient);
				break;
			case dPACKET_SEND_MSG:
				break;
			}

			memmove(&pClient->m_recvBuff[0],
					&pClient->m_recvBuff[msgSize],
					pClient->m_recvSize - msgSize);

			pClient->m_recvSize -= msgSize;
		}
		return 1;
	}


}

// EOF
