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


	cout<<"你好，欢迎聊天！"<<endl;

	{
		cout<<"请输入本地IP（点分十进制）:"<<endl;

		string strMyAddr;
		//do 
		//{
			cin>>strMyAddr;
		//	if (strMyAddr.empty())
		//		cout<<"输入错误，请重新输入！"<<endl;
		//} while (strMyAddr.empty());

		cout<<"请输入本地端口："<<endl;
		short siMyPort;
		cin>>siMyPort;

		cout<<"请输入您的网名："<<endl;
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
		cout<<"请输入对方IP（点分十进制）:"<<endl;

		string strHisAddr;
		//do 
		//{
			cin>>strHisAddr;
		//	if (strHisAddr.empty())
		//		cout<<"输入错误，请重新输入！"<<endl;
		//} while (strHisAddr.empty());

		cout<<"请输入对方端口："<<endl;
		short siHisPort;
		cin>>siHisPort;

		cout<<"请输入他的网名："<<endl;
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
	
	cout<<"您来作为服务器？"<<endl;
	cout<<"1	是"<<endl;
	cout<<"0	否"<<endl;
	int iIsSer;
	do 
	{
		cin<<iIsSer;
		if (1 == iIsSer)
		{
			// 服务器
			ChatObj.SetAsServer();
			cout<<"开始监听！"<<endl;
			if (enum_chat_error != ChatObj.ConnectReady())
			{
				cout<<"连接成功，开始聊天！"<<endl;
			}
		}else if (0 == iIsSer)
		{
			// 客户端
			ChatObj.SetAsClient();
			cout<<"开始连接！"<<endl;
			ChatObj.ConnectReady();
		}else
		{
			cout<<"输入错误，重新输入！"<<endl;
		}
	} while (iIsSer != 1 && iIsSer != 0);

	// Chat Begin
	while(1)
	{
		char bufSend[50];
		memset(bufSend, 0, 50);
		cin.getline(bufSend, 50);
		if (string(bufSend) == "stopChat")
			break;
		else
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

	cout<<"聊天结束！"

    // Wait
    cin.get();
    cin.get();
    
    // End
    return 0;
}
