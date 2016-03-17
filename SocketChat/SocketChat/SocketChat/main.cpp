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
		//do 
		//{
			cin>>strMyAddr;
		//	if (strMyAddr.empty())
		//		cout<<"输入错误，请重新输入！"<<endl;
		//} while (strMyAddr.empty());

		cout<<"Input your port:"<<endl;
		short siMyPort;
		cin>>siMyPort;

		cout<<"Input your screen name:"<<endl;
		string strMyName;
		//do 
		//{
			cin>>strMyName;
		//	if (strMyName.empty())
		//		cout<<"输入错误，请重新输入！"<<endl;
		//} while (strMyName.empty());

		mine.siPort = siMyPort;
		mine.strAddr = strMyAddr;
		mine.strUserName = strMyName;
		cout<<endl;
	}

	{
		cout<<"Please input Other IP Address(decimal with dot):"<<endl;

		string strHisAddr;
		//do 
		//{
			cin>>strHisAddr;
		//	if (strHisAddr.empty())
		//		cout<<"输入错误，请重新输入！"<<endl;
		//} while (strHisAddr.empty());

		cout<<"Input his port:"<<endl;
		short siHisPort;
		cin>>siHisPort;

		cout<<"Input his screen name:"<<endl;
		string strHisName;
		//do 
		//{
			cin>>strHisName;
		//	if (strHisName.empty())
		//		cout<<"输入错误，请重新输入！"<<endl;
		//} while (strHisName.empty());

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
		cout<<"Into Set server"<<endl;
		cin>>iIsSer;
		if (1 == iIsSer)
		{
			// 服务器
			ChatObj.SetAsServer();
			cout<<"Begin Listen!"<<endl;

		}else if (0 == iIsSer)
		{
			// 客户端
			ChatObj.SetAsClient();
			cout<<"Begin Connect!"<<endl;
			ChatObj.ConnectReady();
		}else
		{
			cout<<"Input error, Please again!"<<endl;
		}
	} while (iIsSer != 1 && iIsSer != 0);

	// While accept in server
	cout<<"now, next is server connect"<<endl;
	int retAccept = enum_chat_error;
	while(1 == iIsSer && (retAccept != enum_chat_ok))
	{
		retAccept = ChatObj.ConnectReady();
	}
	if (enum_chat_error != retAccept && (iIsSer == 1))
	{
		cout<<"Connect Success, Chat!"<<endl;
	}

	cout<<"next is chat"<<endl;
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
			ChatObj.ChatMsg(bufSend, 50);
			ChatObj.ShowMsg(bufSend, 50, ChatObj.GetMyName());
		}

		char bufRecv[50];
		memset(bufRecv, 0, 50);
		int nRecvRet = ChatObj.ListenMsg(bufRecv, 50);
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
