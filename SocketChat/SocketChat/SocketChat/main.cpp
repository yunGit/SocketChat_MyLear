//
//  main.cpp
//  SocketChat
//
//  Created by 曹国运 on 16/3/15.
//  Copyright © 2016年 曹国运. All rights reserved.
//

#include <iostream>
#include "Platform.h"
#include "Socket/Socket_yun.hpp"

#define RESAULT_ERROR	-1

#define SEVER_PORT		9000
#define SEVER_IP		"127.0.0.1"

using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

	SOCKET sBind_fd, sConnet_fd;
	SOCKADDR_IN addrClient;

	// 初始化
	YUN::InitSocket();

	// 创建
	if (enum_socket_error == YUN::CreateSocket(sBind_fd))
		return RESAULT_ERROR;

	// 绑定
	if (enum_socket_error == YUN::BindSocket(sBind_fd, SEVER_PORT, SEVER_IP))
		return RESAULT_ERROR;

	// 监听
	if (enum_socket_error == YUN::ListenSocket(sBind_fd, 5))
		return RESAULT_ERROR;

	// 接收
	if (enum_socket_error == YUN::AcceptSocket(sBind_fd, sConnet_fd, addrClient))
		return RESAULT_ERROR;
	
    
    // Wait
    cin.get();
    cin.get();
    
    // End
    return 0;
}
