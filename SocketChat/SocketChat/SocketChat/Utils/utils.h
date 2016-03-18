#ifndef UTILS_H_
#define UTILS_H_

/*
	FILE : utils.h

	各种函数
*/

/*
	日志的画面输出和文件记录函数
*/
#include "Platform.h"

#ifndef __PLATFORM_WIN32__
#include <sys/time.h>
#endif

namespace YUN
{

void log( char *buf, ... );

/*
	Win32 timeGetTime 函数的效仿 (emulation)
*/
#ifndef __PLATFORM_WIN32__
DWORD timeGetTime();
#endif	// __PLATFORM_WIN32__

}


#endif