//
// Created by kenhuang on 19-10-20.
//

#ifndef  Debug_H
#define  Debug_H
/**
 * http://blog.chinaunix.net/uid-28458801-id-4581042.html
 */
#include <assert.h>
#ifdef __DEBUG__
    #define _DEBUG(format,...) \
    printf("\033[4m[ File:%s,Function:%s,Line:%d ]=> ",__FILE__, __FUNCTION__, __LINE__); \
    printf("\033[0m"#format, ##__VA_ARGS__);  \
    printf("\r\n")
    #define _ERROR(format,...) \
    printf("\033[1m\033[41;37m[ File:%s,Function:%s,Line:%d ]=> ",__FILE__, __FUNCTION__, __LINE__); \
    printf("\033[0m"#format, ##__VA_ARGS__);  \
    printf("\r\n")
    #define _ASSERT(exp) \
    if (!(exp)) { \
        printf( "[ %s ]  \r\n",#exp); \
        assert(exp); \
    }
#else
    #define _DEBUG(format...)
    #define _ERROR(format...)
    #define _ASSERT(exp)
#endif
#endif //Debug_H
