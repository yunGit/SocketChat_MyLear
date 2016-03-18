#ifndef UTILS_H_
#define UTILS_H_

/*
	FILE : utils.h

	���ֺ���
*/

/*
	��־�Ļ���������ļ���¼����
*/
#include "Platform.h"

#ifndef __PLATFORM_WIN32__
#include <sys/time.h>
#endif

namespace YUN
{

void log( char *buf, ... );

/*
	Win32 timeGetTime ������Ч�� (emulation)
*/
#ifndef __PLATFORM_WIN32__
DWORD timeGetTime();
#endif	// __PLATFORM_WIN32__

}


#endif