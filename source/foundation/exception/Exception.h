//
// Created by kenhuang on 19-10-20.
//

#ifndef OOPC_EXCEPTION_H
#define OOPC_EXCEPTION_H
#include "../Object.h"
#include <setjmp.h>

#define try(exception) if(!setjmp(*Exception_getBuffer(exception)))
#define catch else
#define finally
#define throw(exception) longjmp(*Exception_getBuffer(exception),(exception)->type)
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
    UNKNOWN_TYPE_EXCEPTION,
    SEGMENT_EXCEPTION
}ExceptionEnum;

PUBLIC(Exception,Object)
    char* message;
    ExceptionEnum type;
END

PUBLIC_STATIC(Exception,Object)
#if defined(__POLYMORPHISM__)
    Exception* (*init)(Exception* self);
    jmp_buf* (*getBuffer)(Exception* self);
#endif
END
Exception* Exception_init(Exception* self);
jmp_buf* Exception_getBuffer(Exception* self);

#endif //OOPC_EXCEPTION_H
