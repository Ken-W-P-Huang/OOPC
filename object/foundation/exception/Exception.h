//
// Created by kenhuang on 2018/6/8.
//
#include "../Object.h"
#ifndef  Exception_H
#define  Exception_H
#include <setjmp.h>
#define try(exception) if(!setjmp(*Exception_getBuffer(exception)))
#define throw(exception)  longjmp(*Exception_getBuffer(exception),exception->type)
#define catch else
#define finally
typedef enum {
    EOF_EXCEPTION = 1,
    FILE_NOT_FOUND_EXCEPTION,
    ARRITHMETIC_EXCEPTION,
    MISSING_RESOUCE_EXCEPTION,
    NULL_POINTER_EXCEPTION,
    ILLEGALARGUMENT_EXCEPTION,
    ARRARY_INDEX_OUT_OF_BOUNDS_EXCEPTION,
    FIELD_TYPE_NOT_MATCH,
    FIELD_NO_ACCESS,
    FIELD_NOT_FOUND,
    UNKNOWN_TYPE_EXCEPTION
}ExceptionType;

/* 类声明*/
public_start(Exception,Object)
    char* message;
    ExceptionType type;
    struct __Exception* private;
public_end

/* 公有方法*/
Exception* Exception_new(Exception* self);
void * catchException (Exception* self);
jmp_buf* Exception_getBuffer(Exception* self);
#endif //Exception_H

