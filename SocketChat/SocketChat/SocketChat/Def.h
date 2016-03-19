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
//					华丽的分割线 - 下面是学习部分
//
//////////////////////////////////////////////////////////////////////////

#define dSERVER_PORT		7000
#define dMAX_LISTEN			200

/*
当使用 select 时，为了重新设置 FD_SETSIZE
限制，而进行的 define。
当时 Linux 或 FreeBSD 时，默认值是 1024；
是 Win32时，默认值是64
*/
// FD_SETSIZE 是指定作为 select 函数的参数传递的fd_set 数据可处理套接字编号范围的参数。
// 如果指定的套接字编号大于该值，则不能通过select 函数的控制输入输出
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
为Win32 中的开发和遍历的 define, typedef
*/

/*
在 UNIX 系列操作系统中利用 close 函数关闭套接字
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