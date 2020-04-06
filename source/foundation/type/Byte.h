//
// Created by kenhuang on 2019-10-23.
//

#ifndef OOPC_BYTE_H
#define OOPC_BYTE_H

#include "../Object.h"

PUBLIC(Byte,Object)

END

PUBLIC_STATIC(Byte,Object)
    const byte MIN_VALUE;
    const byte MAX_VALUE;
#if defined(__POLYMORPHISM__)
    Byte *(*const init)(Byte *self,byte value);
    byte (*const getValue)(Byte*self);
#endif
END
Byte *Byte_init(Byte *self,byte value);
byte Byte_getValue(Byte*self);
#endif //OOPC_BYTE_H
