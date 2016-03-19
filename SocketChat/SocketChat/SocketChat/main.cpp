//
//  main.cpp
//  SocketChat
//
//  Created by 曹国运 on 16/3/15.
//  Copyright © 2016年 曹国运. All rights reserved.
//

#include <iostream>
#include <string>
#include "Platform.h"
#include "Chat/Chat_yun.h"

#include "Def.h"
#include "structs.h"
#include "linked_list.h"
#include "Network\network.h"
#include "Utils\utils.h"
#include "Message\message.h"


#define RESAULT_ERROR	-1

using namespace std;
using namespace YUN;

// 全局变量
int g_ProcessLife = TRUE;

// 服务器的基本套接字
SOCKET g_SOCK = INVALID_SOCKET;

// 客户数据链表
sPCLIENT_DATA g_ClientList = NULL;

// 总连接者数
int g_TotalClient = 0;

// 全局函数
void ProcessLoop();


// Win32 的 Main 处理
#ifdef WIN32
//
//#define dAPP_NAME		"CHAT_Server"
//
//HINSTANCE		hInst;		// Current instance
//HWND			g_hwnd;		// Window Handle
//
//// Windows 消息过程
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM)

int main()
{
	ProcessLoop();
	return TRUE;
}

#else
// 当发生服务器程序结束的Signal时调用的函数
void DestroySignal(int sigNum)
{
	g_ProcessLife = FALSE;
}

// 为 Signal 句柄，初始化Signal的函数
void InitSignal()
{
	// 防止因 SIGPIPE signal 而引擎的墙纸结束服务器运行的问题儿编写的程序代码s
	struct sigaction act;
	act.sa_handler = SIG_IGN;
	act.sa_flags &= -SA_RESETHAND;
	sigaction(SIGPIPE, &act, NULL);

	// 当发生强制结束服务器运行的Signal时，调用正常结束服务器运行函数的 Signal 的设置
	signal(SIGINT, DestroySignal);
	signal(SIGKILL, DestroySignal);
	signal(SIGQUIT, DestroySignal);
	signal(SIGTERM, DestroySignal);
}

// Linux、FreeBSD的Main 处理
int main()
{
	InitSignal();
	ProcessLoop();
	return TRUE;
}


#endif	// WIN32

// 服务器基本数据初始化函数
void InitServerData()
{

}

// 服务器基本函数删除函数
void DestroyServerData()
{

}

// Win32、FreeBSD 和 Linux 功能的主循环
void ProcessLoop()
{
#ifdef WIN32
	// 当是 WIN32时，利用WSAStartup 初始化 Winsock DLL.
	WSADATA WSAData;
	WSAStartup(0x0101, &WSAData);

	// 用于Windows消息句柄的Msg变量
	MSG msg;
#endif

	// 服务器套接字初始化
	g_SOCK = InitServerSock(dSERVER_PORT, dMAX_LISTEN);
	
	if (g_SOCK == INVALID_SOCKET)
		return;

	// 服务器数据初始化
	InitServerData();
	
	// FD_ZERO 将set清零使结合中不含任何fd
	// FD_SET  将fd加入set集合
	// FD_CLR  将fd从set集合中清除
	// FD_ISSET 在调用select()函数后，用FD_ISSET来检测fd是否在set结合中，当检测到fd在set中则返回真，否则返回假
	fd_set read_set;
	fd_set write_set;
	fd_set exc_set;
	struct timeval tv;
	SOCKET nfds;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	sPCLIENT_DATA client, next_client;

	while(g_ProcessLife)
	{
#ifdef WIN32
		// WIN32消息句柄
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
		}
#endif
		// fd_set 初始化
		FD_ZERO(&read_set);
		FD_ZERO(&write_set);
		FD_ZERO(&exc_set);

		FD_ZERO(g_SOCK, &read_set);
		nfds = g_SOCK;

		// 求套接字的最大值
		LIST_WHILE(g_ClientList, client, next_client, m_next);

		if (client->m_sock >nfds)
			nfds = client->m_sock;
		
		FD_SET(client->m_sock, &read_set);
		FD_SET(client->m_sock, &write_set);
		FD_SET(client->m_sock, &exc_set);
		LIST_WHILEEND(g_ClientList, client, next_client);

		// select
		if (select(nfds + 1, &read_set, &write_set, &exc_set, &tv) < -1)
		{
			log("select Error!\r\n");
			continue;
		}

		// 新连接请求的处理
		if (FD_ISSET(g_SOCK, &read_set))
			AcceptNewClient(g_SOCK);

		// 例外错误处理和数据 recv(接收)
		LIST_WHILE(g_ClientList, client, next_client, m_next);
		
		// 错误
		// 下面这段代码我认为会有错误，因为还没有对该socket进行select()
		// 2016/3/19	YUN
		if (FD_ISSET(client->m_sock, &exc_set))
		{
			DisconnectClient(client);
			LIST_SKIP(client, next_client);
		}

		// 超过1分钟没有什么反应时，断开连接
		if (client->m_lastRecvTime + 60000 <= timeGetTime())
		{
			DisconnectClient(client);
			LIST_SKIP(client, next_client);
		}
			
		// 当存在可recv 的数据时
		if (FD_ISSET(client->m_sock, &read_set))
		{
			if (!RecvFromClient(client))
			{
				DisconnectClient(client);
				LIST_SKIP(client, next_client);
			}
		}

		// 当存在已经recv 的数据时
		if (client->m_recvSize)
		{
			if (!ReadRecvBuff(client))
			{
				DisconnectClient(client);
				LIST_SKIP(client, next_client);
			}
		}
		LIST_WHILEEND(g_ClientList, client, next_client);

		// Send Buff Flush
		LIST_WHILE(g_ClientList, client, next_client, m_next)

		if (client->m_sendSize && FD_ISSET(client->m_sock, &write_set))
		{
			if (FlushSendBuff(client) < 0)
			{
				DisconnectClient(client);
				LIST_SKIP(client, next_client);
			}
		}

		LIST_WHILEEND(g_ClientList, client, next_client);
	}

	// 服务器数据的删除
	DestroyServerData();

	// 释放 Winsock DLL
#ifdef WIN32
	WSACleanup();
#endif
}

// EOF


//////////////////////////////////////////////////////////////////////////
//	
//					Next is by Yun
//
//////////////////////////////////////////////////////////////////////////

//int main(int argc, const char * argv[]) {
//
//	Chat_yun ChatObj;
//	USER_INFO mine, his;
//
//
//	cout<<"Hello , welcome to Chat!"<<endl;
//
//	{
//		cout<<"Please input your IP Address(decimal with dot):"<<endl;
//
//		string strMyAddr;
//		cin>>strMyAddr;
//
//		cout<<"Input your port:"<<endl;
//		short siMyPort;
//		cin>>siMyPort;
//
//		cout<<"Input your screen name:"<<endl;
//		string strMyName;
//		cin>>strMyName;
//
//		mine.siPort = siMyPort;
//		mine.strAddr = strMyAddr;
//		mine.strUserName = strMyName;
//		cout<<endl;
//	}
//
//	{
//		cout<<"Please input Other IP Address(decimal with dot):"<<endl;
//
//		string strHisAddr;
//		cin>>strHisAddr;
//
//		cout<<"Input his port:"<<endl;
//		short siHisPort;
//		cin>>siHisPort;
//
//		cout<<"Input his screen name:"<<endl;
//		string strHisName;
//		cin>>strHisName;
//
//		his.siPort = siHisPort;
//		his.strAddr = strHisAddr;
//		his.strUserName = strHisName;
//		cout<<endl;
//	}
//
//	ChatObj.SetSelfInfo(mine);
//	ChatObj.SetOtherInfo(his);
//	ChatObj.InitConnect();
//	
//	cout<<"Are you Server?"<<endl;
//	cout<<"1	Yes"<<endl;
//	cout<<"0	No"<<endl;
//	int iIsSer;
//	do 
//	{
//		cin>>iIsSer;
//		if (1 == iIsSer)
//		{
//			// Server
//			ChatObj.SetAsServer();
//			cout<<"Begin Listen!"<<endl;
//
//		}else if (0 == iIsSer)
//		{
//			// Client
//			ChatObj.SetAsClient();
//			cout<<"Begin Connect!"<<endl;
//			ChatObj.ConnectReady();
//		}else
//		{
//			cout<<"Input error, Please again!"<<endl;
//		}
//	} while (iIsSer != 1 && iIsSer != 0);
//
//	// While accept in server
//	int retAccept = enum_chat_error;
//	while(1 == iIsSer && (retAccept != enum_chat_ok))
//	{
//		retAccept = ChatObj.ConnectReady();
//	}
//
//
//	// Chat Begin
//	while(1)
//	{
//		char bufSend[50];
//		memset(bufSend, 0, 50);
//		cin.getline(bufSend, 50);
//		if (string(bufSend) == "stopChat")
//			break;
//		else if (!string(bufSend).empty())
//		{
//			int nLen = string(bufSend).length();
//			ChatObj.ChatMsg(bufSend, nLen);
//			ChatObj.ShowMsg(bufSend, nLen, ChatObj.GetMyName());
//		}
//
//		char bufRecv[50];
//		memset(bufRecv, 0, 50);
//		int nRecvRet = ChatObj.ListenMsg(bufRecv, sizeof(bufRecv));
//		if (enum_chat_error == nRecvRet)
//			continue;
//		else
//			ChatObj.ShowMsg(bufRecv, nRecvRet, ChatObj.GetOtherName());
//	}
//
//	cout<<"Chat end!";
//
//    // Wait
//    cin.get();
//    cin.get();
//    
//    // End
//    return 0;
//}
