/*
	FILE : utils.cpp

	各种有用的函数
*/

#include "..\Def.h"
#include <stdarg.h>

namespace YUN
{

/*
	利用 va_list, 使日志的画面输出和记录函数
	使用 sprintf 等格式成为可能
*/
void log( char *buf, ... )
{
	char tmp_buf[1024], tmp_buf2[1024];
	va_list args;	// 是在C语言中解决便餐问题的一组宏，所在头文件：#include <stdarg.h>,用于获取不确定个数的参数
	FILE *fp;

	time_t tval;
	time( &tval );
	struct tm *currTM = localtime( &tval );

	fp = fopen( "log.txt", "a" );

	if (!fp)
		return;

	va_start(args, buf);
	vsprintf(tmp_buf, buf, args);
	va_end(args);

	sprintf(tmp_buf2, "[%d/%02d/%02d %02d:%02d:%02d]%s",
		currTM->tm_year+1900,
		currTM->tm_mon+1,
		currTM->tm_mday,
		currTM->tm_hour,
		currTM->tm_min,
		currTM->tm_sec,
		tmp_buf );

	strcpy( tmp_buf, tmp_buf2 );
	fprintf(fp, tmp_buf);

#ifndef __PLATFORM_WIN32__
	printf(tmp_buf);
#endif

	fclose(fp);
	
}

/*
	在 Linux、FreeBSD 中，没有以 ms（毫秒）为单位获取CPU时间的
	Win32的timeGetTime函数
*/
#ifndef __PLATFORM_WIN32__
DWORD timeGetTime()
{
	struct timeval tv;			// 注：tv.tv_usec;代指的是微秒
	gettimeofday(&tv, NULL);	// 用以获得系统的精准时间
	DWORD value = (((tv.tv_sec - 1000000000) * 1000) + (tv.tv_usec / 1000));
	return value;
}
#endif

}