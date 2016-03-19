/*
	FILE : network.cpp

	用于套接字处理的源程序文件
*/

#include "..\Def.h"
#include "..\Platform.h"
#include "..\structs.h"
#include "..\linked_list.h"
#include "..\Network\network.h"
#include "..\Utils\utils.h"

namespace YUN
{

	// 服务器主套接字
	extern SOCKET g_SOCK;

	// 客户数据链表
	extern sPCLIENT_DATA g_ClientList;

	// 总连接数
	extern int g_TotalClient;

#ifdef WIN32
	// 设置Win32 用套接字为 nonblock 模式
	void nonblock( SOCKET s )
	{
		u_long			ulOn = 1L;

		// ioctlsocket() 是控制套接口的模式 #include <winsock.h>
		// param1, 套接字描述字
		// param2, 对套接字s的操作命令
		// param3, 只想该命令所带参数的指针
		ioctlsocket(s, FIONBIO, (unsigned long*)&ulOn);
	}

#else
	// 设置Linux、FreeBSD 用套接字为 nonblock 模式
	void nonblock( SOCKET s )
	{
		int flags;

		flags = fcntl(s, F_GETFL, 0);
		flags |= O_NONBLOCK;
		if (fcntl(s, F_SETFL, flags) < 0)
			log("* nonblock Error\r\n");
	}
#endif	// WIN32

	// 当服务器 listen 套接字的初始化失败时，返回 -1
	// param2, 等待连接队列的最大长度
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

		// 设置成，即使因错误而非正常结束，也能立刻用同样的地址进行 bind
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

		// LINGER 的设定
		struct linger ld;

		ld.l_onoff = 0;
		ld.l_linger = 0;
		if (setsockopt(sock, SOL_SOCKET,SO_LINGER, (char*)&ld, sizeof(ld)) < 0)
			log("* Error: sersockopt SO_LINGER ... \r\n");

		// 结构体初始化
		memset((char*)&sa, 0, sizeof(sa));

		sa.sin_family = PF_INET;
		sa.sin_port = htons(port);
		sa.sin_addr.S_addr = INADDR_ANY;

		// 绑定
		if (bind(sock, (struct sockaddr *)&sa, sizeof(sa)) < 0)
		{
			log("InitServerSock(), bind(...) failed .. [PORT:%d]\r\n", port);
			closesocket(sock);
			return -1;
		}

		// 设置已创建的套接字为Nonblocking 模式
		nonblock(sock);

		// 设置套接字处于等待状态
		// param2, 等待连接队列的最大长度
		listen(sock, backLog);
		
		return sock;
	}

	// 新的连接处理请求
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

		// 客户数据的初始化
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

		// 链表连接
		INSERT_TO_LIST(g_ClientList, newClient, newClient->m_prev, m_next);
		++g_ClientList;

		log("Accept New Connection: %d [%s]\r\n", newDesc, newClient->m_Ip);

	}

	// 把要发送的数据复制到发送缓冲器
	void SendData(sPCLIENT_DATA pClient, const char *data, int size)
	{
		if ((pClient->m_sendSize + size) > dMAX_SOCK_BUFF)
			return;

		memcpy(&pClient->m_sendBuff[pClient->m_sendSize], data, size);
		pClient->m_sendSize += size;		
	}

	// 给所有在连接中的客户发送数据
	void SendToAll(const char *data, int size)
	{
		sPCLIENT_DATA client, next_client;

		LIST_WHILE(g_ClientList, client, next_client, m_next);
		SendData(client, data, size);
		LIST_WHILEEND(g_ClientList, client, next_client);
	}

	// 清空发送缓冲器
	int FlushSendBuff(sPCLIENT_DATA pClient)
	{
		int sendSize;

		sendSize = send(pClient->m_sock, pClient->m_sendBuff, pClient->m_sendSize, 0);

		// Error
		if (sendSize <= 0)
			return -1;

		// 如果已经发送的数据比应该发送的数据少，则只处理已经发送的数据
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

	// 断开连接
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
			// 出现出 Nonblock 之外的错误时，结束连接状态
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
