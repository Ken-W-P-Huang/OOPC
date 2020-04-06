#include <csetjmp>
#include "gtest/gtest.h"


extern "C"{
    extern struct ClassType * const Class;
    extern void * alloc (const ClassType * clazz);
    extern  ClassType* const ExceptionClass;
    struct Exception* Exception_init(Exception* self);
    jmp_buf* Exception_getBuffer(Exception* self);
}

static void triggerError(Exception* exception){
    longjmp(*Exception_getBuffer(exception),10);

}

TEST(TEST, Exception) {
    Exception* exception = Exception_init((Exception*)alloc(ExceptionClass));
    printf("\n%p\n",Exception_getBuffer(exception));
    if (setjmp(*Exception_getBuffer(exception)) == 0) {
        printf("Executing\n");
        triggerError(exception);
        printf("should not print!\n");
    }
    printf("Error:xxx");
}