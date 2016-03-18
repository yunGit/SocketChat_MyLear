/*
	FILE : network.cpp

	�����׽��ִ����Դ�����ļ�
*/

#include "Def.h"
#include "Platform.h"
#include "structs.h"
#include "linked_list.h"
#include "network.h"
#include "utils.h"

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

		// ���óɣ���ʱ������������������Ҳ��������ͬ���ĵ�ַ���� bind
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

		sPCLIENT_DATA newClient = (sPCLIENT_DATA)malloc(sizeof(sPCLIENT_DATA));
		if (!newClient)
		{
			closesocket(newClient);
			return;
		}

		// �ͻ����ݵĳ�ʼ��
		newClient->m_sock = newDesc;

		//strcpy(newClient->)

	}

}
