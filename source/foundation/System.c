//
// Created by kenhuang on 2019-11-19.
//
//#ifdef _WIN32
//#include <windows.h>
//#else
//#include <unistd.h>
//#include <signal.h>
//#include <zconf.h>
//#endif
//
//#include "System.h"
//
//PRIVATE(System)
//
//END
///***********************************************************************************************************************
// *  私有方法
// **********************************************************************************************************************/
//static void deinit(System *self) {
//
//}
//
///***********************************************************************************************************************
// *  公有方法
// **********************************************************************************************************************/
//System *System_init(System *self) {
//    if (Object_init((Object *) self)) {
//
//    }
//    return self;
//}
//
//
//
//#ifdef _WIN32
////define something for Windows (32-bit and 64-bit, this part is common)
//   #ifdef _WIN64
//      //define something for Windows (64-bit only)
//   #else
//      //define something for Windows (32-bit only)
//   #endif
//#elif __APPLE__
//#include "TargetConditionals.h"
//#if TARGET_IPHONE_SIMULATOR
//// iOS Simulator
//#elif TARGET_OS_IPHONE
//// iOS device
//#elif TARGET_OS_MAC
//// Other kinds of Mac OS
//#else
//#   error "Unknown Apple platform"
//#endif
//#elif __linux__
//// linux
//#elif __unix__ // all unices not caught above
//    // Unix
//#elif defined(_POSIX_VERSION)
//    // POSIX
//#else
//#   error "Unknown compiler"
//#endif
//
//
//
//
//static unsigned long long getTotalSystemMemory(){
//#ifdef _WIN32
//    MEMORYSTATUSEX status;
//    status.dwLength = sizeof(status);
//    GlobalMemoryStatusEx(&status);
//    return status.ullTotalPhys;
//#else
//    long pages = sysconf(_SC_PHYS_PAGES);
//    long page_size = sysconf(_SC_PAGE_SIZE);
//    return (unsigned long long)pages * page_size;
//#endif
//}
//
//typedef enum {
//    unknown,
//    lowToHigh,
//    highToLow
//}StackDirectionEnum;
//static StackDirectionEnum getStackGrowDirection(){
//    static char *p = NULL;
//    char c;
//    if (p == NULL) {
//        p = &c;
//        return getStackGrowDirection();
//    }
//
////        printf("First in stack address is %p.\n", p);
////        printf("Second in stack address is %p.\n", &c);
//    if (&c > p) {
//        printf("Stack grows from low address to high address!\n");
//        return lowToHigh;
//    }
//
//    printf("Stack grows from high address to low address!\n");
//    return highToLow;
//}
///***********************************************************************************************************************
// *  静态方法
// **********************************************************************************************************************/
//STATIC_INIT(System)
//END