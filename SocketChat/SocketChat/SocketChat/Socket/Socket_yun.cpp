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
		addr.sin_addr.S_un.S_addr = HTONL(lIp);	
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
		int ret = connect(s, (SOCKADDR*)&addrClt, nLen);
		if (enum_socket_error == ret)
		{
			cout<<"connect error"<<endl;
			CloseSocket(s);

			return enum_socket_error;
		}
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
			cout<<"recv error"<<endl;
			CloseSocket(s);

			return enum_socket_success;
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
