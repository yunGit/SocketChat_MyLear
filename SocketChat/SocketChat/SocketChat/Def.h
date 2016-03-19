#ifndef DEF_H_
#define DEF_H_

#include <iostream>
#include "Platform.h"

using namespace std;

enum SOCKET_RESAULT
{
	enum_socket_error = -1,
	enum_socket_success,
	enum_socket_true,
	enum_socket_false,
};

enum SOCKET_SELECT_TYPE
{
	enum_select_read = 0,
	enum_select_write,
	enum_select_exc,
};

enum CHAR_RESAULT
{
	enum_chat_error = -1,
	enum_chat_ok,
};

typedef struct
{
	string	strUserName;
	string	strAddr;
	short	siPort;
}USER_INFO,*PUSER_INFO;

//////////////////////////////////////////////////////////////////////////
//				
//					�����ķָ��� - ������ѧϰ����
//
//////////////////////////////////////////////////////////////////////////

#define dSERVER_PORT		7000
#define dMAX_LISTEN			200

/*
��ʹ�� select ʱ��Ϊ���������� FD_SETSIZE
���ƣ������е� define��
��ʱ Linux �� FreeBSD ʱ��Ĭ��ֵ�� 1024��
�� Win32ʱ��Ĭ��ֵ��64
*/
// FD_SETSIZE ��ָ����Ϊ select �����Ĳ������ݵ�fd_set ���ݿɴ����׽��ֱ�ŷ�Χ�Ĳ�����
// ���ָ�����׽��ֱ�Ŵ��ڸ�ֵ������ͨ��select �����Ŀ����������
#define	FD_SETSIZE			6000

#ifdef __PLATFORM_WIN32__

#include <windows.h>
#include <winsock.h>
#include <mmsystem.h>
#include <stdio.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")
//#include <inaddr.h>
//#include "resource.h"

#else

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

/*
ΪWin32 �еĿ����ͱ����� define, typedef
*/

/*
�� UNIX ϵ�в���ϵͳ������ close �����ر��׽���
*/
#define closesocket(sock)	close(sock)

#define INVALID_SOCKET	(-1)
#define TRUE	1
#define FALSE	0

typedef int					SOCKET;
typedef	bool				BOOL;
typedef unsigned char		BYTE;
typedef unsigned long		DWORD;
typedef unsigned short		WORD;

#endif	// End of __PLATFORM_WIN32__

#endif	// End of DEF_H_

//EOF