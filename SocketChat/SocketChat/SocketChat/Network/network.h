#ifndef NETWORK_H_
#define NETWORK_H_

/*
	FILE : network.h

	�׽��ִ�����ͷ
*/
#include "..\Platform.h"

namespace YUN
{
	// �����׽���Ϊ Noblocking ģʽ�ĺ���
	void nonblock( SOCKET s );

	// ��ʼ�������� listen �׽��ֵĺ���
	SOCKET InitServerSock(int port, int backLog);

	// �µ���������Ĵ���
	void AcceptNewClient(SOCKET MotherSocket);

	// �Ͽ�����
	void DisconnectClient(sPCLIENT_DATA pClient);

	// �Ѵ��͵����ݸ��Ƶ�������
	void SendData(sPCLIENT_DATA pClient, const char* data, int size);

	// �������������еĿͻ��˷�������
	void SendToAll(const char *data, int size);

	// ��շ��ͻ�����
	int FlushSendBuff(sPCLIENT_DATA pClient);

	// recv
	BOOL RecvFromClient(sPCLIENT_DATA pClient);

}

#endif

// EOF