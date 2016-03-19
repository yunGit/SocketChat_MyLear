/*
	FILE : utils.cpp

	�������õĺ���
*/

#include "..\Def.h"
#include <stdarg.h>

namespace YUN
{

/*
	���� va_list, ʹ��־�Ļ�������ͼ�¼����
	ʹ�� sprintf �ȸ�ʽ��Ϊ����
*/
void log( char *buf, ... )
{
	char tmp_buf[1024], tmp_buf2[1024];
	va_list args;	// ����C�����н����������һ��꣬����ͷ�ļ���#include <stdarg.h>,���ڻ�ȡ��ȷ�������Ĳ���
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
	�� Linux��FreeBSD �У�û���� ms�����룩Ϊ��λ��ȡCPUʱ���
	Win32��timeGetTime����
*/
#ifndef __PLATFORM_WIN32__
DWORD timeGetTime()
{
	struct timeval tv;			// ע��tv.tv_usec;��ָ����΢��
	gettimeofday(&tv, NULL);	// ���Ի��ϵͳ�ľ�׼ʱ��
	DWORD value = (((tv.tv_sec - 1000000000) * 1000) + (tv.tv_usec / 1000));
	return value;
}
#endif

}