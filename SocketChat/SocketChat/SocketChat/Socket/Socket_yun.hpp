//
//  Socket_yun.hpp
//  SocketChat
//
//  Created by 曹国运 on 16/3/15.
//  Copyright © 2016年 曹国运. All rights reserved.
//

#ifndef Socket_yun_hpp
#define Socket_yun_hpp

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../Platform.h"
#include "../Def.h"

// --------   Mac  -----------
#ifdef __PLATFORM_MAC__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef sockaddr*	LPSOCKADDR;
typedef sockaddr_in SOCKADDR_IN;
typedef int			SOCKET;
typedef LPSOCKADDR    SOCKADDR;
#endif

// -------  Win32  ------------
#ifdef __PLATFORM_WIN32__
#include <sys/types.h>
#include <WinSock.h>

// 下面的操作等同于 Property -> .. -> Linker-> command line
#pragma comment( lib, "ws2_32.lib" )	// 链接Winsock2.h的静态库文件

// 将主机字节顺序转换为网络字节顺序
#define HTONS(hostshort)		htons(hostshort)	// (to network short)
#define HTONL(hostlong)			htonl(hostlong)		// (to network long)
// 将网络地址转换为点分十进制的字符串指针
#define INET_NTOA(in_addr_in)	inet_ntoa(in_addr_in)
#endif
// ----------------------------

#define dMAX_CLIENT		5

//#define SOCKET_ERROR	-1

using namespace std;

namespace YUN {
    // Socket C++

	// Init Socket
	void InitSocket();
    
    // Create Socket
    SOCKET_RESAULT CreateSocket(SOCKET &s);
    
    // Bind Socket
	// INADDR_ANY -> 0.0.0.0
	// 表示不确定地址，或“任意地址”
    SOCKET_RESAULT BindSocket(SOCKET s, short siPort, string strIp = "127.0.0.1");
	SOCKET_RESAULT BindSocket(SOCKET s, short siPort, long lIp = INADDR_ANY);

	// Listen Socket
	SOCKET_RESAULT ListenSocket(SOCKET s, int nListenNum);

	// Connect Socket - for Client
	SOCKET_RESAULT ConnectSocket(SOCKET s, SOCKADDR_IN &addrClt);

	// Accept Socket - for Server
	SOCKET_RESAULT AcceptSocket(SOCKET s, SOCKET& sBindConnect, SOCKADDR_IN &addrClt);

#ifdef __PLATFORM_WIN32__
	// Resert Socket READ&WRITE&EXC Set - Server
	void ResetSocketSelectSet(SOCKET& s);

	// Select Socket Buff - Server
	// param is SOCKET_SELECT_TYPE
	SOCKET_RESAULT SelectSocketBuff(SOCKET &s, int nSelectType);
#endif


	// Send Buff to Socket
	// return is size of copy or SOCKET_ERROR
	int SendBuffToSocket(SOCKET s, const char* strBuf, int nLen, int nFlag = 0);

	// Recv Buff from Socket
	// return is size of copy or SOCKET_ERROR
	int RecvBuffToSocket(SOCKET s, char* strBuf, int nLen, int nFlag = 0);

	// Close Socket
	void CloseSocket(SOCKET s);

	// Clean Socket
	void CleanSocket();
}

#endif /* Socket_yun_hpp */
