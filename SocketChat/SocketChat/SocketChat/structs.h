#ifndef STRUCT_H_
#define STRUCT_H_

/*
	FILE : structs.h

	各种结构体头
*/

#define dMAX_SOCK_BUFF		1024
#define dNAME_LEN			50
#define dIP_LEN				20

struct client_data
{
	/*
		用于数据传送的套接字等各种数据变量
	*/
	SOCKET		m_sock;

	char		m_Ip[dIP_LEN];

	char		m_recvBuff[dMAX_SOCK_BUFF];
	int			m_recvSize;
	int			m_recvPos;

	char		m_sendBuff[dMAX_SOCK_BUFF];
	int			m_sendSize;

	/*
		对话名
	*/
	char		m_Name[dNAME_LEN];

	/*
		用于连接状态确认的最后 recv 时间值
	*/
	DWORD		m_lastRecvTime;

	/*
		用于链表连接的指针
	*/
	struct client_data *m_prev;
	struct client_data *m_next;
};

/*
	方便使用 struct client_data 的 typedef
*/
typedef struct client_data sCLIENT_DATA, *sPCLIENT_DATA;

#endif

// EOF