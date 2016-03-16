//
//  Socket_yun.hpp
//  SocketChat
//
//  Created by 曹国运 on 16/3/15.
//  Copyright © 2016年 曹国运. All rights reserved.
//

#ifndef Socket_yun_hpp
#define Socket_yun_hpp

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../Platform.h"

#ifdef __PLATFORM_MAC__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef sockaddr* LPSOCKADDR;
typedef sockaddr_in SOCKADDR_IN;
typedef int SOCKET;
#endif

#ifdef __PLATFORM_WIN32__
#include <sys/types.h>
#include <winsock2.h>

#define HTONS(hostshort)	htons(hostshort)
#endif

using namespace std;

namespace YUN {
    // Socket C++
    
    // Create Socket
    SOCKET CreateSocket(void);
    
    // Bind Socket
    SOCKET BindSocket(SOCKET s, string strIp, short siPort);
}

#endif /* Socket_yun_hpp */
