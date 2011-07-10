#ifndef _DEBUG_MESSAGE_H_
#define _DEBUG_MESSAGE_H_
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif	//__cplusplus

#ifndef _CXX_TEST_
#	define TRACEF(x, ...) printf(x, __VA_ARGS__);
#	define TRACE(s) printf(s);
#else
#	define TRACEF(x, ...)
#	define TRACE(s)
#endif

#ifdef __cplusplus
}
#endif	//__cplusplus


#endif
