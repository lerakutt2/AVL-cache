#ifndef WIN_GET_TIME_OF_DAY_HEADER_INCLUDED
#define WIN_GET_TIME_OF_DAY_HEADER_INCLUDED

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h> // portable: uint64_t   MSVC: __int64 

// MSVC defines this in winsock2.h!?
typedef struct timeval {
	long tv_sec;
	long tv_usec;
} timeval;

int gettimeofday(struct timeval* tp, struct timezone* tzp);

#endif // #ifdef _WIN32
#endif // #ifndef WIN_GET_TIME_OF_DAY_HEADER_INCLUDED
