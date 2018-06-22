
#ifndef  Debug_H
#define  Debug_H

#include <assert.h>
#define _DEBUG(msg...) printf("[ %s,%s, %d ]=>",__FILE__, __FUNCTION__, __LINE__); printf(msg);printf("\r\n")
#define _ERROR(msg...) printf("[ error: %s, %d]=>", __FILE__, __LINE__);printf(msg); printf("\r\n")
#define _ASSERT(exp) \
if (!(exp)) { \
printf( "[ %s ]  ",#exp); \
printf("\r\n");assert(exp); \
}

#endif //Debug_H