//
//  Socket_yun.cpp
//  SocketChat
//
//  Created by 曹国运 on 16/3/15.
//  Copyright © 2016年 曹国运. All rights reserved.
//

#include "Socket_yun.hpp"

namespace YUN {
    // Socket C++

	// Init Socket
	void InitSocket()
	{
#ifdef __PLATFORM_WIN32__
		// 初始化阶段调用WSAStartup来初始化Sockets DLL
		// wsaData 用来存储系统传回的WinSocket的信息
		WSADATA wsaData;	
		WSAStartup(MAKEWORD(2,2), &wsaData);

		// LOBYTE() 取得16禁止数最低位
		// HIBYTE() 取得16禁止数最高（最左边）那个字节的内容
		//if (LOBYTE( wsaData.wVersion ) != 1 || HIBYTE( wsaData.wVersion ) != 1)
		//{
		//	WSACleanup();
		//	return;
		//}
#endif
	}
    
	// Create Socket
    SOCKET_RESAULT CreateSocket(SOCKET &s)
    {
        s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	// 0
		if (enum_socket_error == s)
		{
			cout<<"socket init error"<<endl;
			return enum_socket_error;
		}

		
		int nNetTimeout = 10000;	// 1s
		// 设置接收超时时间
		setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&nNetTimeout, sizeof(int));
		//// 设置发送超时时间
		//setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char*)&nNetTimeout, sizeof(int));
        
        return enum_socket_success;
    }
    
	// Bind Socket
    SOCKET_RESAULT BindSocket(SOCKET s, short siPort, string strIp)
    {
        SOCKADDR_IN addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(strIp.c_str());
        addr.sin_port = HTONS(siPort);
        
        int ret = bind(s, (LPSOCKADDR)&addr, sizeof(addr));
		if (enum_socket_error == ret)
		{
			cout<<"socket bind error"<<endl;
			CloseSocket(s);

			return enum_socket_error;
		}
        return enum_socket_success;
    }
	SOCKET_RESAULT BindSocket(SOCKET s, short siPort, long lIp)
	{
		SOCKADDR_IN addr;
		addr.sin_family = AF_INET;
#ifdef __PLATFORM_WIN32__
		addr.sin_addr.S_un.S_addr = HTONL(lIp);
#endif
#ifdef __PLATFORM_MAC__
        addr.sin_addr.s_addr = HTONL(lIp);
#endif
		addr.sin_port = HTONS(siPort);

		int ret = bind(s, (LPSOCKADDR)&addr, sizeof(addr));
		if (enum_socket_error == ret)
		{
			cout<<"socket bind error"<<endl;
			CloseSocket(s);

			return enum_socket_error;
		}
		return enum_socket_success;
	}

	// Listen Socket
	SOCKET_RESAULT ListenSocket(SOCKET s, int nListenNum)
	{
		int ret = listen(s, nListenNum);	// param 2 最好是系统提供的默认值
		if (enum_socket_error == ret)
		{
			cout<<"listen error"<<endl;
			CloseSocket(s);

			return enum_socket_error;
		}
		return enum_socket_success;
	}

	// Connect Socket - for Client
	SOCKET_RESAULT ConnectSocket(SOCKET s, SOCKADDR_IN &addrClt)
	{
		int nLen = sizeof(SOCKADDR);

		// 为一个链接提供服务
		// addrClient 包含了发出连接请求的客户机IP地址信息
		connect(s, (SOCKADDR*)&addrClt, nLen);
		//int ret = connect(s, (SOCKADDR*)&addrClt, nLen);
		//if (enum_socket_error == ret)
		//{
		//	cout<<"connect error"<<endl;
		//	CloseSocket(s);

		//	return enum_socket_error;
		//}
		return enum_socket_success;
	}

	// Accept Socket
	SOCKET_RESAULT AcceptSocket(SOCKET s, SOCKET& sBindConnect, SOCKADDR_IN &addrClt)
	{
		int nLen = sizeof(SOCKADDR);
		
		// 为一个链接请求提供服务
		// addrClient 包含了发出连接请求的客户机IP地址信息
		// 返回的新 socket 描述服务器与该客户机的连接
		sBindConnect = accept(s, (SOCKADDR*)&addrClt, &nLen);
		if (sBindConnect == enum_socket_error)
		{
			cout<<"accept error"<<endl;
			CloseSocket(s);

			return enum_socket_error;
		}
		return enum_socket_success;
	}

#ifdef __PLATFORM_WIN32__
	// Resert Socket READ&WRITE&EXC Set - Server
	void ResetSocketSelectSet(SOCKET& s)
	{
		// 通过 select 函数，适当时候使用 accept 、 send 、 recv 等函数
		// read_set : 为了确定有没有可 recv() 数据而使用
		// write_set : 为了确定能不能使用 send() 而使用
		// exc_set : 为了例外处而使用
		fd_set read_set, write_set, exc_set;


		// 各个fd_set值初始化成NULL
		FD_ZERO(&read_set);
		FD_ZERO(&write_set);
		FD_ZERO(&exc_set);

		// s 是经过 socket() -> bind()->listen()函数，准备好接收客户连接请求的套接字
		// 给客户端套接字设置 read_set、write_set、exc_set 等三个值。
		// 通过这一操作，使 select 函数能获取对应套接字中要检测出的信号。
		FD_SET(s, &read_set);
		FD_SET(s, &write_set);
		FD_SET(s, &exc_set);				
	}

	// Select Socket Buff - server
	SOCKET_RESAULT SelectSocketBuff(SOCKET &s, int nSelectType)
	{
		// 通过 select 函数，适当时候使用 accept 、 send 、 recv 等函数
		// read_set : 为了确定有没有可 recv() 数据而使用
		// write_set : 为了确定能不能使用 send() 而使用
		// exc_set : 为了例外处而使用
		fd_set read_set, write_set, exc_set;
		struct timeval	tv;	// 时间点

		// 为了当没有任何信号时跳过去，设置所有的成员为0
		// tv_sec 是以 s 为单位的时间，而 tv_usec是ms为单位的时间
		tv.tv_sec = 0;
		tv.tv_usec = 0;

		// 使用 select() 函数
		if (select(s + 1, &read_set, &write_set, &exc_set, &tv) == SOCKET_ERROR)
			return enum_socket_error;
		
		SOCKET_RESAULT ret;
		switch(nSelectType)
		{
		case enum_select_read:
			ret = (FD_ISSET(s, &read_set)) ? enum_socket_true : enum_socket_false;
		case enum_select_write:
			ret = (FD_ISSET(s, &write_set)) ? enum_socket_true : enum_socket_false;
		case enum_select_exc:
			ret = (FD_ISSET(s, &exc_set)) ? enum_socket_true : enum_socket_false;
		}
		
		
	}
#endif



	// Send Buff to Socket
	int SendBuffToSocket(SOCKET s, const char* strBuf, int nLen, int nFlag)
	{
		// param 4 ??
		int ret = send(s, strBuf, nLen, nFlag);
		if (enum_socket_error == ret)
		{
			cout<<"send error"<<endl;
			CloseSocket(s);

			return enum_socket_error;
		}
		return ret;
	}

	// Recv Buff from Socket
	int RecvBuffToSocket(SOCKET s, char* strBuf, int nLen, int nFlag)
	{
		int ret = recv(s, strBuf, nLen, nFlag);
		if (enum_socket_error == ret)
		{
			//cout<<"recv error"<<endl;
			//CloseSocket(s);
			// upper by noted for when recv is timeout can continue socket

			return enum_socket_error;
		}
		return ret;
	}

	// Close Socket
	void CloseSocket(SOCKET s)
	{
		closesocket(s);
		s = NULL;
		cout<<"socket closed"<<endl;
		return;
	}

	// Clean Socket
	void CleanSocket()
	{
#ifdef __PLATFORM_WIN32__
		WSACleanup();
#endif
	}
}
