#ifndef _LIBCFG_H_
#define _LIBCFG_H_


/*use bool instead of tvision's Boolean type*/
#define TVE_USE_BOOL 1

#define LIB_PLATFORM_DPMI32 1
#define LIB_PLATFORM_WIN32  2
#define LIB_PLATFORM_OS2    4

#ifdef WIN32
#define LIB_PLATFORM LIB_PLATFORM_WIN32
#elif defined(DPMI32) /*rewrite this to whatever the DPMI define is*/
#define LIB_PLATFORM LIB_PLATFORM_DPMI32
#elif defined(OS2) /*rewrite this to whatever the OS/2 define is*/
#define LIB_PLATFORM LIB_PLATFORM_OS2
#else
#error A platform must be defined!
#endif

#define MAX_VIEW_WIDTH 300 /*view maximum width, used for TDrawBuffer::bufer*/

#define TVE_KEYS /*use more keys than the standard implementation defines*/

#ifdef WIN32
#define __FLAT__  /*nothing*/

#define _FARFUNC  /*nothing*/

#define _Cdecl _cdecl

#endif


#endif
