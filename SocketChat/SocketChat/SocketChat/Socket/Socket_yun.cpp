//
//  Socket_yun.cpp
//  SocketChat
//
//  Created by 曹国运 on 16/3/15.
//  Copyright © 2016年 曹国运. All rights reserved.
//

#include "Socket_yun.hpp"

namespace YUN {
    // Socket C++
    
    Socket CreateSocket(void)
    {
        Socket s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        
        return s;
    }
    
    Socket BindSocket(Socket s, string strIp, short siPort)
    {
        SOCKADDR_IN addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(strIp.c_str());
        addr.sin_port = HTONS(siPort);
        
        bind(s, (LPSOCKADDR)&addr, sizeof(addr));
        return s;
    }
}
