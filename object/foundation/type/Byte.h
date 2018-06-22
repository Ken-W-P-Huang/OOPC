//
// Created by kenhuang on 2018/6/13.
//

#ifndef  Byte_H
#define  Byte_H
#include "../Object.h"
public_start(ByteT, Object)

/* 公有属性 */
    struct __ByteT* private;
public_end
/* 静态属性 */
extern const byte BYTE_MIN_VALUE;
extern const byte BYTE_MAX_VALUE;
/* 公有方法*/
ByteT* Byte_new(ByteT*self,byte value);
byte Byte_getValue(ByteT*self);
/* 静态方法 */
void ByteTClazz_fillFieldsMethods();

#endif //Byte_H












