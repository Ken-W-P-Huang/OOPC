//
// Created by kenhuang on 2018/6/20.
//

#include "../Object.h"
#ifndef  Access_H
#define  Access_H


public_start(Access, Object)
/* 公有属性 */
struct __Access *private;
public_end
extern const Access* READABLE_WRITABLE_PRIVATE;
extern const Access* READABLE_UNWRITABLE_PRIVATE;
extern const Access* READABLE_UNWRITABLE_PUBLIC;
extern const Access* READABLE_WRITABLE_PUBLIC;
extern const Access* UNREADABLE_WRITABLE_PRIVATE;
extern const Access* UNREADABLE_UNWRITABLE_PRIVATE;
extern const Access* UNREADABLE_UNWRITABLE_PUBLIC;
extern const Access* UNREADABLE_WRITABLE_PUBLIC;
/* 公有方法*/
bool Access_getReadable(Access* self);
bool Access_getWritable(Access* self);
bool Access_getIsPrivate(Access* self);

/* 静态方法 */
void AccessClazz_fillFieldsMethods();

#endif //Access_H