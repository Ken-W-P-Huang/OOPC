//
// Created by kenhuang on 2019-10-21.
//
#include "../Object.h"
#ifndef OOPC_ACCESS_H
#define OOPC_ACCESS_H

PUBLIC(Access,Object)
END

PUBLIC_STATIC(Access,Object)
    Access* const READABLE_WRITABLE_PRIVATE;
    Access* const READABLE_UNWRITABLE_PRIVATE;
    Access* const READABLE_UNWRITABLE_PUBLIC;
    Access* const READABLE_WRITABLE_PUBLIC;
    Access* const UNREADABLE_WRITABLE_PRIVATE;
    Access* const UNREADABLE_UNWRITABLE_PRIVATE;
    Access* const UNREADABLE_UNWRITABLE_PUBLIC;
    Access* const UNREADABLE_WRITABLE_PUBLIC;
#if defined(__POLYMORPHISM__)
    Access *(*init)(Access *self,bool readable,bool writable,bool isPrivate);
    bool (*getReadable)(Access* self);
    bool (*getWritable)(Access* self);
    bool (*getIsPrivate)(Access* self);
#endif
END

Access *Access_init(Access *self,bool readable,bool writable,bool isPrivate);
bool Access_getReadable(Access* self);
bool Access_getWritable(Access* self);
bool Access_getIsPrivate(Access* self);

#endif //OOPC_ACCESS_H

