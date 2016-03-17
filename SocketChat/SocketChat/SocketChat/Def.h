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

#endif