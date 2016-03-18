#ifndef STRUCT_H_
#define STRUCT_H_

/*
	FILE : structs.h

	���ֽṹ��ͷ
*/

#define dMAX_SOCK_BUFF		1024
#define dNAME_LEN			50
#define dIP_LEN				20

struct client_data
{
	/*
		�������ݴ��͵��׽��ֵȸ������ݱ���
	*/
	SOCKET		m_sock;

	char		m_Ip[dIP_LEN];

	char		m_recvBuff[dMAX_SOCK_BUFF];
	int			m_recvSize;
	int			m_recvPos;

	char		m_sendBuff[dMAX_SOCK_BUFF];
	int			m_sendSize;

	/*
		�Ի���
	*/
	char		m_Name[dNAME_LEN];

	/*
		��������״̬ȷ�ϵ���� recv ʱ��ֵ
	*/
	DWORD		m_lastRecvTime;

	/*
		�����������ӵ�ָ��
	*/
	struct client_data *m_prev;
	struct client_data *m_next;
};

/*
	����ʹ�� struct client_data �� typedef
*/
typedef struct client_data sCLIENT_DATA, *sPCLIENT_DATA;

#endif

// EOF