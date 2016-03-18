/*
	FILE : network.cpp

	用于套接字处理的源程序文件
*/

#include "Def.h"
#include "Platform.h"
#include "structs.h"
#include "linked_list.h"
#include "network.h"
#include "utils.h"

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

		// 设置成，及时因错误而非正常结束，也能立刻用同样的地址进行 bind
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

		sPCLIENT_DATA newClient = (sPCLIENT_DATA)malloc(sizeof(sPCLIENT_DATA));
		if (!newClient)
		{
			closesocket(newClient);
			return;
		}

		// 客户数据的初始化
		newClient->m_sock = newDesc;

		//strcpy(newClient->)

	}

}
