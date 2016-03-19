/*
	FILE : network.cpp

	�����׽��ִ����Դ�����ļ�
*/

#include "..\Def.h"
#include "..\Platform.h"
#include "..\structs.h"
#include "..\linked_list.h"
#include "..\Network\network.h"
#include "..\Utils\utils.h"

namespace YUN
{

	// ���������׽���
	extern SOCKET g_SOCK;

	// �ͻ���������
	extern sPCLIENT_DATA g_ClientList;

	// ��������
	extern int g_TotalClient;

#ifdef WIN32
	// ����Win32 ���׽���Ϊ nonblock ģʽ
	void nonblock( SOCKET s )
	{
		u_long			ulOn = 1L;

		// ioctlsocket() �ǿ����׽ӿڵ�ģʽ #include <winsock.h>
		// param1, �׽���������
		// param2, ���׽���s�Ĳ�������
		// param3, ֻ�����������������ָ��
		ioctlsocket(s, FIONBIO, (unsigned long*)&ulOn);
	}

#else
	// ����Linux��FreeBSD ���׽���Ϊ nonblock ģʽ
	void nonblock( SOCKET s )
	{
		int flags;

		flags = fcntl(s, F_GETFL, 0);
		flags |= O_NONBLOCK;
		if (fcntl(s, F_SETFL, flags) < 0)
			log("* nonblock Error\r\n");
	}
#endif	// WIN32

	// �������� listen �׽��ֵĳ�ʼ��ʧ��ʱ������ -1
	// param2, �ȴ����Ӷ��е���󳤶�
	SOCKET InitServerSock(int port, int backLog)
	{
		struct sockaddr_in	sa;
		SOCKET				sock;
		
		sock = socket(PF_INET, SOCK_STREAM, 0);

		if (sock < 0)
		{
			log("InitServerSock(), socket(..) failed [PORT:%d] .. \r\n", port);
			return -1;
		}

		// ���óɣ���ʹ������������������Ҳ��������ͬ���ĵ�ַ���� bind
#ifndef WIN32
		int opt = 1;

		if (setsockopt(sock,
						SOL_SOCKET,
						SO_REUSEADDR,
						(char*)&opt,
						sizeof(opt) < 0))
		{
			log("* Error: setsockopt REUSEADDR\r\n");
			exit(1);
		}
#endif	// WIN32

		// LINGER ���趨
		struct linger ld;

		ld.l_onoff = 0;
		ld.l_linger = 0;
		if (setsockopt(sock, SOL_SOCKET,SO_LINGER, (char*)&ld, sizeof(ld)) < 0)
			log("* Error: sersockopt SO_LINGER ... \r\n");

		// �ṹ���ʼ��
		memset((char*)&sa, 0, sizeof(sa));

		sa.sin_family = PF_INET;
		sa.sin_port = htons(port);
		sa.sin_addr.S_addr = INADDR_ANY;

		// ��
		if (bind(sock, (struct sockaddr *)&sa, sizeof(sa)) < 0)
		{
			log("InitServerSock(), bind(...) failed .. [PORT:%d]\r\n", port);
			closesocket(sock);
			return -1;
		}

		// �����Ѵ������׽���ΪNonblocking ģʽ
		nonblock(sock);

		// �����׽��ִ��ڵȴ�״̬
		// param2, �ȴ����Ӷ��е���󳤶�
		listen(sock, backLog);
		
		return sock;
	}

	// �µ����Ӵ�������
	void AcceptNewClient(SOCKET MotherSocket)
	{
		SOCKET newDesc;
		struct sockaddr_in peer;
#ifdef WIN32
		int peersize;
#else
		socklen_t peersize;
#endif

		peersize = sizeof(peer);

		newDesc = accept(MotherSocket, (struct sockaddr *)&peer, &peersize);

		if (newDesc < 0)
		{
			log("Accept New Client Failed!\r\n");
			return ;
		}

		// socket nonblocking
		nonblock(newDesc);

		//sPCLIENT_DATA newClient = (sPCLIENT_DATA)malloc(sizeof(sPCLIENT_DATA));
		sPCLIENT_DATA newClient = new sCLIENT_DATA;
		if (!newClient)
		{
			closesocket(newClient->m_sock);
			return;
		}

		// �ͻ����ݵĳ�ʼ��
		newClient->m_sock = newDesc;

		strcpy(newClient->m_Ip, (char*)inet_ntoa(peer.sin_addr));

		*newClient->m_recvBuff = '\0';
		newClient->m_recvSize = 0;
		newClient->m_recvPos = 0;

		*newClient->m_sendBuff = '\0';
		newClient->m_sendSize = 0;

		*newClient->m_Name = '\0';
		newClient->m_lastRecvTime = timeGetTime();

		newClient->m_prev = NULL;
		newClient->m_next = NULL;

		// ��������
		INSERT_TO_LIST(g_ClientList, newClient, newClient->m_prev, m_next);
		++g_ClientList;

		log("Accept New Connection: %d [%s]\r\n", newDesc, newClient->m_Ip);

	}

	// ��Ҫ���͵����ݸ��Ƶ����ͻ�����
	void SendData(sPCLIENT_DATA pClient, const char *data, int size)
	{
		if ((pClient->m_sendSize + size) > dMAX_SOCK_BUFF)
			return;

		memcpy(&pClient->m_sendBuff[pClient->m_sendSize], data, size);
		pClient->m_sendSize += size;		
	}

	// �������������еĿͻ���������
	void SendToAll(const char *data, int size)
	{
		sPCLIENT_DATA client, next_client;

		LIST_WHILE(g_ClientList, client, next_client, m_next);
		SendData(client, data, size);
		LIST_WHILEEND(g_ClientList, client, next_client);
	}

	// ��շ��ͻ�����
	int FlushSendBuff(sPCLIENT_DATA pClient)
	{
		int sendSize;

		sendSize = send(pClient->m_sock, pClient->m_sendBuff, pClient->m_sendSize, 0);

		// Error
		if (sendSize <= 0)
			return -1;

		// ����Ѿ����͵����ݱ�Ӧ�÷��͵������٣���ֻ�����Ѿ����͵�����
		if (sendSize < pClient->m_sendSize)
		{
			memmove(&pClient->m_sendBuff[0],
					&pClient->m_sendBuff[sendSize],
					pClient->m_sendSize - sendSize);
			pClient->m_sendSize -= sendSize;
		}
		else
		{
			*pClient->m_sendBuff = '\0';
			pClient->m_sendSize = 0;
		}
		return sendSize;
	
	}

	// �Ͽ�����
	void DisconnectClient(sPCLIENT_DATA pClient)
	{
		REMOVE_FROM_LIST(g_ClientList, pClient, m_prev, m_next);
		if (pClient->m_sock != INVALID_SOCKET)
			closesocket(pClient->m_sock);

		//free(pClient);
		delete pClient;
		pClient = NULL;
		
	}

	// recv
	BOOL RecvFromClient(sPCLIENT_DATA pClient)
	{
		int recvSize;
		char recvBuff[dMAX_SOCK_BUFF];

		recvSize = recv(pClient->m_sock, recvBuff, dMAX_SOCK_BUFF, 0);

		if (recvSize == 0)
			return 0;
		
		if (recvSize < 0)
		{
			// ���ֳ� Nonblock ֮��Ĵ���ʱ����������״̬
#ifdef WIN32
			if (WSAGetLastError() != WSAEWOULDBLOCK)
				return 0;
#else
			if (errno != EWOULDBLOCK)
				return 0;
#endif
			else
				return 1;
		}

		// Buffer Overflow
		if ((pClient->m_recvSize + recvSize) >= dMAX_SOCK_BUFF)
			return 0;

		pClient->m_lastRecvTime = timeGetTime();

		memcpy(&pClient->m_recvBuff[pClient->m_recvSize], recvBuff, recvSize);
		pClient->m_recvSize += recvSize;

		return 1;
	}

}

// EOF
