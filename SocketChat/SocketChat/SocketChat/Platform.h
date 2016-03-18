#ifdef WIN32

#define __PLATFORM_WIN32__


typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef int				BOOL;
typedef float			FLOAT;

#else

#define __PLATFORM_MAC__

typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef int				BOOL;
typedef float			FLOAT;

#endif
