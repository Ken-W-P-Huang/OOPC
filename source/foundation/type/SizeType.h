//
// Created by kenhuang on 2019-12-02.
//

#ifndef OS_SIZETYPE_H
#define OS_SIZETYPE_H

#include "../Object.h"

PUBLIC(SizeType, Object)
END

PUBLIC_STATIC(SizeType, Object)
#if defined(__POLYMORPHISM__)

#endif
END

SizeType *SizeType_init(SizeType *self,size_t value);
size_t SizeType_getValue(SizeType* self);
#endif //OS_SIZETYPE_H
