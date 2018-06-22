//
// Created by kenhuang on 2018/6/8.
//
#include <pthread.h>
#ifndef  type_H
#define  type_H
typedef  unsigned char      boolean;     /* Boolean value type. */
typedef  unsigned long int  uint32;      /* Unsigned 32 bit value */
typedef  unsigned short     uint16;      /* Unsigned 16 bit value */
typedef  unsigned char      uint8;       /* Unsigned 8  bit value */
typedef  signed long int    int32;       /* Signed 32 bit value */
typedef  signed short       int16;       /* Signed 16 bit value */
typedef  signed char        int8;        /* Signed 8  bit value */
#define byte  unsigned char
#ifndef __cplusplus
#define bool  unsigned char
#define true 1
#define false 0
#elif defined(__GNUC__) && !defined(__STRICT_ANSI__)
/* Define _Bool, bool, false, true as a GNU extension. */
#define _Bool bool
#define bool  bool
#define false false
#define true  true
#endif
#define __bool_true_false_are_defined 1
/***********************************************************************************************************************
 *  得到指定地址上的一个字节或字
 **********************************************************************************************************************/
#define  MEM_B(x) (*((byte *)(x)))
#define  MEM_W(x) (*((word *)(x)))
/***********************************************************************************************************************
 *  求最大值和最小值
 **********************************************************************************************************************/
#define  MAX(x,y) (((x)>(y)) ? (x) : (y))
#define  MIN(x,y) (((x) < (y)) ? (x) : (y))
/***********************************************************************************************************************
 *  得到一个字的高位和低位字节
 **********************************************************************************************************************/
#define WORD_LO(xxx)  ((byte) ((word)(xxx) & 255))
#define WORD_HI(xxx)  ((byte) ((word)(xxx) >> 8))
/***********************************************************************************************************************
 *  将一个字母转换为大写
 **********************************************************************************************************************/
#define UPCASE(c) (((c)>='a' && (c) <= 'z') ? ((c) – 0×20) : (c))
/***********************************************************************************************************************
 *  判断字符是不是10进值的数字
 **********************************************************************************************************************/
#define  DECCHK(c) ((c)>='0' && (c)<='9')
/***********************************************************************************************************************
 *  判断字符是不是16进值的数字
 **********************************************************************************************************************/
#define HEXCHK(c) (((c) >= '0' && (c)<='9') ((c)>='A' && (c)<= 'F') \((c)>='a' && (c)<='f'))
/***********************************************************************************************************************
 *  防止溢出的一个方法
 **********************************************************************************************************************/
#define INC_SAT(val) (val=((val)+1>(val)) ? (val)+1 : (val))
/***********************************************************************************************************************
 *  返回数组元素的个数
 **********************************************************************************************************************/
#define ARR_SIZE(a)  (sizeof((a))/sizeof((a[0])))
/***********************************************************************************************************************
 *  返回一个无符号数n尾的值MOD_BY_POWER_OF_TWO(X,n)=X%(2^n)
 **********************************************************************************************************************/
#define MOD_BY_POWER_OF_TWO( val, mod_by ) ((dword)(val) & (dword)((mod_by)-1))
/***********************************************************************************************************************
 *  对于IO空间映射在存储空间的结构,输入输出处理
 **********************************************************************************************************************/
#define inp(port) (*((volatile byte *)(port)))
#define inpw(port) (*((volatile word *)(port)))
#define inpdw(port) (*((volatile dword *)(port)))
#define outp(port,val) (*((volatile byte *)(port))=((byte)(val)))
#define outpw(port, val) (*((volatile word *)(port))=((word)(val)))
#define outpdw(port, val) (*((volatile dword *)(port))=((dword)(val)))
/***********************************************************************************************************************
 *  类定义宏
 **********************************************************************************************************************/
#define public_start(subclass,super)  \
extern const Class1 * subclass##Clazz; \
void Class_register##subclass##Clazz(); \
void Class_unregister##subclass##Clazz(); \
typedef struct subclass subclass; \
struct subclass { \
    /* 继承自##super## */ \
    super _; \

    /* 公共属性 */

#define public_end };

#define private_start(subclass,super) \
typedef struct __##subclass __##subclass; \
struct __##subclass{
#define private_end };

#define class_start(subclass,super,poffset,psize) \
static void Class_init(Class1*clazz);\
static void Class_deinit(Class1*clazz);\
const Class1 * subclass##Clazz; \
void Class_register##subclass##Clazz() { \
    subclass##Clazz = (Class1 *) Class_newInstance(Clazz, \
                                            #subclass, \
                                            super##Clazz, \
                                            sizeof(subclass), \
                                            psize,\
                                            poffset,\
                                            (Object* (*)(Object *, va_list *)) init, \
                                            (Object* (*)(Object *)) deinit); \
     Class_init((Class1 *)subclass##Clazz); \
} \
void Class_unregister##subclass##Clazz() { \
    Class_deinit((Class1 *)subclass##Clazz); \
    Object_destroy((void *) subclass##Clazz); \
    subclass##Clazz = NULL; \
}
#define class_end

#endif //type_H
