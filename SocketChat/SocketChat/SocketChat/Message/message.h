#ifndef MESSAGE_H_
#define MESSAGE_H_

/*
	FILE : message.h
*/

#define dPACKET_ALIVE_CHECK			0		// ��������״̬��ȷ��
#define dPACKET_CHANGE_NICK			1		// �ºŵ��޸�
#define dPACKET_RECV_MSG			2		// ������Ϣ���ͻ� -> ��������
#define dPACKET_SEND_MSG			3		// ������Ϣ�������� -> �ͻ���

BOOL	ReadRecvBuff(sPCLIENT_DATA pClient);

#endif

// EOF