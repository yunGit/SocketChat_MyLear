#ifndef NETWORK_H_
#define NETWORK_H_

/*
	FILE : network.h

	套接字处理函数头
*/
#include "..\Platform.h"

namespace YUN
{
	// 设置套接字为 Noblocking 模式的函数
	void nonblock( SOCKET s );

	// 初始化服务器 listen 套接字的函数
	SOCKET InitServerSock(int port, int backLog);

	// 新的连接请求的处理
	void AcceptNewClient(SOCKET MotherSocket);

	// 断开连接
	void DisconnectClient(sPCLIENT_DATA pClient);

	// 把传送的数据复制到缓冲器
	void SendData(sPCLIENT_DATA pClient, const char* data, int size);

	// 给所有在连接中的客户端发送数据
	void SendToAll(const char *data, int size);

	// 清空发送缓冲器
	int FlushSendBuff(sPCLIENT_DATA pClient);

	// recv
	BOOL RecvFromClient(sPCLIENT_DATA pClient);

}

#endif

// EOF