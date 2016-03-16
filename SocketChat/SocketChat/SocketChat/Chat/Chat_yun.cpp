#include "Chat_yun.h"

namespace YUN
{
	BOOL		Chat_yun::SetSelfInfo(const USER_INFO& info)
	{
		if (!IsInfoValid(info))
			return enum_chat_error;
		
		m_SelfInfo.strAddr		= info.strAddr;
		m_SelfInfo.siPort		= info.siPort;
		m_SelfInfo.strUserName	= info.strUserName;
		return enum_chat_ok;
	}
	BOOL		Chat_yun::SetOtherInfo(const USER_INFO& info)
	{
		if (!IsInfoValid(info))
			return enum_chat_error;

		m_OtherInfo.strAddr		= info.strAddr;
		m_OtherInfo.siPort		= info.siPort;
		m_OtherInfo.strUserName = info.strUserName;
		return enum_chat_ok;

	}
	BOOL		Chat_yun::InitConnect()
	{
		if (!IsInfoValid(m_SelfInfo))
			return enum_chat_error;

		InitSocket();
		
		if (enum_socket_error == CreateSocket(m_SelfS))
		{
			cout<<"Chat, InitConnect Create error!"<<endl;
			return enum_chat_error;
		}
		if (enum_socket_error == BindSocket(m_SelfS, m_OtherInfo.siPort, m_OtherInfo.strAddr))
		{
			cout<<"Chat, InitConnect Bind error!"<<endl;
			return enum_chat_error;
		}
		return enum_chat_ok;
	}
	BOOL		Chat_yun::SetAsServer()
	{
		if (!IsInfoValid(m_OtherInfo))
			return enum_chat_error;

		if (enum_socket_error == ListenSocket(m_SelfS, 5))
		{
			cout<<"Chat, SetAsServer Listen error!"<<endl;
			return enum_chat_error;
		}
		m_bIsServer = true;
		return enum_chat_ok;
	}
	BOOL		Chat_yun::SetAsClient()
	{
		if (!IsInfoValid(m_OtherInfo))
			return enum_chat_error;

		m_bIsServer = false;
		return enum_chat_ok;
	}
	BOOL		Chat_yun::ConnectReady()
	{
		if (m_bIsServer == true)
		{
			SOCKADDR_IN addrClient;
			if (enum_socket_error == AcceptSocket(m_SelfS, m_OtherS, addrClient))
				return enum_chat_error;	
			else
				ChatMsg("连接成功，开始聊天");
		}else
		{
			SOCKADDR_IN addrClient;
			if (enum_socket_error == ConnectSocket(m_SelfS, addrClient))
				return enum_chat_error;		
		}
		return enum_chat_ok;
	}
	BOOL		Chat_yun::ChatMsg(const char* buf, int nLen)
	{
		SOCKET tmpSocket = m_bIsServer ? m_OtherS : m_SelfS;
		int ret = SendBuffToSocket(tmpSocket, buf, nLen);
		if (enum_socket_error == ret)
		{
			cout<<"Send Buff Error!"<<endl;
			return enum_chat_error;
		}
		return enum_chat_ok;
	}
	BOOL		Chat_yun::ListenMsg(const char* buf, int nLen)
	{
		SOCKET tmpSocket = m_bIsServer ? m_OtherS : m_SelfS;
		int ret = RecvBuffToSocket(tmpSocket, buf, nLen);
		if (enum_socket_error == ret)
		{
			cout<<"Listen Buff Error!"<<endl;
			return enum_chat_error;
		}
		if (ret == 0)
		{
			return enum_chat_error;
		}
		return enum_chat_ok;
	}
	void		Chat_yun::ShowMsg(const char* buf, int nLen, string strUserName)
	{
		cout<<endl;
		cout<<strUserName<<" said:"<<endl;
		cout<<buf<<endl;
	}
	BOOL		Chat_yun::BreakConnect()
	{
		CloseSocket(m_SelfS);
		CleanSocket();
	}


	const char* Chat_yun::GetMyName()
	{
		return m_SelfInfo.strUserName.c_str();
	}

	const char* Chat_yun::GetOtherName()
	{
		return m_OtherInfo.strUserName.c_str();
	}

	void Chat_yun::CleanAll()
	{
		m_SelfInfo.strAddr.empty();
		m_OtherInfo.strAddr.empty();
		m_SelfInfo.strUserName.empty();
		m_OtherInfo.strUserName.empty();
		m_SelfInfo.siPort = 0;
		m_OtherInfo.siPort = 0;
		m_SelfS = m_OtherS = 0;
		m_bIsServer = false;
	}

	bool Chat_yun::IsInfoValid(USER_INFO& info)
	{
		return (!info.strAddr.empty() && info.siPort != 0 && !info.strUserName.empty());
	}
	
	Chat_yun::Chat_yun()
	{
		CleanAll();
	}

	Chat_yun::~Chat_yun()
	{
		CleanAll();
	}

	Chat_yun::Chat_yun(Chat_yun& other)
	{
	}
}