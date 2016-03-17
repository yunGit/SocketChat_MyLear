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

#define RESAULT_ERROR	-1

using namespace std;
using namespace YUN;

int main(int argc, const char * argv[]) {

	Chat_yun ChatObj;
	USER_INFO mine, his;


	cout<<"Hello , welcome to Chat!"<<endl;

	{
		cout<<"Please input your IP Address(decimal with dot):"<<endl;

		string strMyAddr;
		cin>>strMyAddr;

		cout<<"Input your port:"<<endl;
		short siMyPort;
		cin>>siMyPort;

		cout<<"Input your screen name:"<<endl;
		string strMyName;
		cin>>strMyName;

		mine.siPort = siMyPort;
		mine.strAddr = strMyAddr;
		mine.strUserName = strMyName;
		cout<<endl;
	}

	{
		cout<<"Please input Other IP Address(decimal with dot):"<<endl;

		string strHisAddr;
		cin>>strHisAddr;

		cout<<"Input his port:"<<endl;
		short siHisPort;
		cin>>siHisPort;

		cout<<"Input his screen name:"<<endl;
		string strHisName;
		cin>>strHisName;

		his.siPort = siHisPort;
		his.strAddr = strHisAddr;
		his.strUserName = strHisName;
		cout<<endl;
	}

	ChatObj.SetSelfInfo(mine);
	ChatObj.SetOtherInfo(his);
	ChatObj.InitConnect();
	
	cout<<"Are you Server?"<<endl;
	cout<<"1	Yes"<<endl;
	cout<<"0	No"<<endl;
	int iIsSer;
	do 
	{
		cin>>iIsSer;
		if (1 == iIsSer)
		{
			// Server
			ChatObj.SetAsServer();
			cout<<"Begin Listen!"<<endl;

		}else if (0 == iIsSer)
		{
			// Client
			ChatObj.SetAsClient();
			cout<<"Begin Connect!"<<endl;
			ChatObj.ConnectReady();
		}else
		{
			cout<<"Input error, Please again!"<<endl;
		}
	} while (iIsSer != 1 && iIsSer != 0);

	// While accept in server
	int retAccept = enum_chat_error;
	while(1 == iIsSer && (retAccept != enum_chat_ok))
	{
		retAccept = ChatObj.ConnectReady();
	}


	// Chat Begin
	while(1)
	{
		char bufSend[50];
		memset(bufSend, 0, 50);
		cin.getline(bufSend, 50);
		if (string(bufSend) == "stopChat")
			break;
		else if (!string(bufSend).empty())
		{
			int nLen = string(bufSend).length();
			ChatObj.ChatMsg(bufSend, nLen);
			ChatObj.ShowMsg(bufSend, nLen, ChatObj.GetMyName());
		}

		char bufRecv[50];
		memset(bufRecv, 0, 50);
		int nRecvRet = ChatObj.ListenMsg(bufRecv, sizeof(bufRecv));
		if (enum_chat_error == nRecvRet)
			continue;
		else
			ChatObj.ShowMsg(bufRecv, nRecvRet, ChatObj.GetOtherName());
	}

	cout<<"Chat end!";

    // Wait
    cin.get();
    cin.get();
    
    // End
    return 0;
}
