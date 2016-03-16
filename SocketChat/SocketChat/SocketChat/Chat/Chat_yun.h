#ifndef CHAT_YUN_H_
#define CHAT_YUN_H_

#include <iostream>
#include "Def.h"
#include "../Socket/Socket_yun.hpp"

using namespace std;

namespace YUN
{
	class Chat_yun
	{
	private:
		USER_INFO	m_SelfInfo, m_OtherInfo;
		SOCKET		m_SelfS, m_OtherS;
		bool		m_bIsServer;

	public:
		BOOL		SetSelfInfo(const USER_INFO& info);
		BOOL		SetOtherInfo(const USER_INFO& info);
		BOOL		InitConnect();
		BOOL		SetAsServer();
		BOOL		SetAsClient();
		BOOL		ConnectReady();
		BOOL		ChatMsg(const char* buf, int nLen);
		BOOL		ListenMsg(const char* buf, int nLen);
		void		ShowMsg(const char* buf, int nLen, string strUserName);
		BOOL		BreakConnect();

		const char* GetMyName();
		const char* GetOtherName();

	private:
		void		CleanAll();
		bool		IsInfoValid(USER_INFO& info);

	public:
		Chat_yun();
		~Chat_yun();

	private:
		Chat_yun(Chat_yun& other);
	};
}

#endif