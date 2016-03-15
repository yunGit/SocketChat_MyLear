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

#define __PLATFORM_MAC__

#ifdef __PLATFORM_MAC__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

typedef int Socket;
typedef sockaddr* LPSOCKADDR;
typedef sockaddr_in SOCKADDR_IN;

using namespace std;

namespace YUN {
    // Socket C++
    
    // Create Socket
    Socket CreateSocket(void);
    
    // Bind Socket
    Socket BindSocket(Socket s, string strIp, short siPort);
}

#endif /* Socket_yun_hpp */
