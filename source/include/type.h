//
// Created by kenhuang on 19-10-20.
//
#ifndef  type_H
#define  type_H
typedef long (* Function)();
typedef  unsigned char      boolean;     /* Boolean value type. */
typedef  unsigned long int  uint32;      /* Unsigned 32 bit value */
typedef  unsigned short     uint16;      /* Unsigned 16 bit value */
typedef  unsigned char      uint8;       /* Unsigned 8  bit value */
typedef  signed long int    int32;       /* Signed 32 bit value */
typedef  signed short       int16;       /* Signed 16 bit value */
typedef  signed char        int8;        /* Signed 8  bit value */
typedef  unsigned char byte;
typedef  unsigned short word;
#define bool  short
#define true 1
#define false 0
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
#define ARAY_SIZE(array)  (sizeof((array))/sizeof((array[0])))
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
 *  面向对象
 **********************************************************************************************************************/
 /* 接口不应当有私有属性 */
#define INTERFACE(sub,super) \
typedef struct sub sub; \
struct sub{ \
    super _;

#define PUBLIC(sub,super) \
typedef struct sub sub; \
struct sub{ \
    super _; \
    struct sub##_P *p;

#define PRIVATE(sub) \
typedef struct sub##_P sub##_P; \
struct sub##_P{

#define PUBLIC_STATIC(sub,super) \
typedef struct sub##ClassType sub##ClassType; \
extern sub##ClassType *const sub##Class; \
struct sub##ClassType{ \
    super##ClassType _;

#define PRIVATE_STATIC(sub) \
typedef struct sub##ClassType##_P sub##ClassType##_P; \
struct sub##ClassType##_P{

#if defined(__CLASS_ATTRIBUTE__)
    #define STATIC_INIT(sub) \
    static void sub##_clinit(); \
    static sub##ClassType sub##Clazz[] = { \
    { \
                    NULL, \
                    #sub, \
                    sizeof(sub), \
                    sizeof(sub##_P), \
                    offsetof(sub, p), \
                    sizeof(sub##ClassType), \
                    REAL, \
                    false, \
                    sub##_clinit, \
                    (void (*)(Object *)) deinit,\
    } \
    }; \
    \
    sub##ClassType *const sub##Class = sub##Clazz; \
    static void sub##_clinit() {

    #define ABSTRACT_STATIC_INIT(sub) \
    static void sub##_clinit(); \
    static sub##ClassType sub##Clazz[] = { \
    { \
                    NULL, \
                    #sub, \
                    sizeof(sub), \
                    sizeof(sub##_P), \
                    offsetof(sub, p), \
                    sizeof(sub##ClassType), \
                    ABSTRACT, \
                    false, \
                    sub##_clinit, \
                    (void (*)(Object *)) deinit,\
    } \
    }; \
    \
    sub##ClassType *const sub##Class = sub##Clazz; \
    static void sub##_clinit() {


    #define INTERFACE_STATIC_INIT(sub) \
    static void sub##_clinit(); \
    static sub##ClassType sub##Clazz[] = { \
    { \
                    NULL, \
                    #sub, \
                    sizeof(sub), \
                    0, \
                    0, \
                    sizeof(sub##ClassType), \
                    INTERFACE, \
                    false, \
                    sub##_clinit, \
                    NULL,\
    } \
    }; \
    \
    sub##ClassType *const sub##Class = sub##Clazz; \
    static void sub##_clinit() {

    #define FINAL_STATIC_INIT(sub) \
    static void sub##_clinit(); \
    static sub##ClassType sub##Clazz[] = { \
    { \
                    NULL, \
                    #sub, \
                    sizeof(sub), \
                    sizeof(sub##_P), \
                    offsetof(sub, p), \
                    sizeof(sub##ClassType), \
                    REAL, \
                    true, \
                    sub##_clinit, \
                    (void (*)(Object *)) deinit,\
    } \
    }; \
    \
    sub##ClassType *const sub##Class = sub##Clazz; \
    static void sub##_clinit() {
#else
    #define STATIC_INIT(sub) \
    static void sub##_clinit(); \
    static sub##ClassType sub##Clazz[] = { \
    { \
                    NULL, \
                    #sub, \
                    sizeof(sub), \
                    sizeof(sub##_P), \
                    offsetof(sub, p), \
                    sizeof(sub##ClassType), \
                    sub##_clinit, \
                    (void (*)(Object *)) deinit,\
    } \
    }; \
    \
    sub##ClassType *const sub##Class = sub##Clazz; \
    static void sub##_clinit() {

    #define ABSTRACT_STATIC_INIT(sub) \
    static void sub##_clinit(); \
    static sub##ClassType sub##Clazz[] = { \
    { \
                    NULL, \
                    #sub, \
                    sizeof(sub), \
                    sizeof(sub##_P), \
                    offsetof(sub, p), \
                    sizeof(sub##ClassType), \
                    sub##_clinit, \
                    (void (*)(Object *)) deinit,\
    } \
    }; \
    \
    sub##ClassType *const sub##Class = sub##Clazz; \
    static void sub##_clinit() {


    #define INTERFACE_STATIC_INIT(sub) \
    static void sub##_clinit(); \
    static sub##ClassType sub##Clazz[] = { \
    { \
                    NULL, \
                    #sub, \
                    sizeof(sub), \
                    0, \
                    0, \
                    sizeof(sub##ClassType), \
                    sub##_clinit, \
                    NULL,\
    } \
    }; \
    \
    sub##ClassType *const sub##Class = sub##Clazz; \
    static void sub##_clinit() {

    #define FINAL_STATIC_INIT(sub) \
    static void sub##_clinit(); \
    static sub##ClassType sub##Clazz[] = { \
    { \
                    NULL, \
                    #sub, \
                    sizeof(sub), \
                    sizeof(sub##_P), \
                    offsetof(sub, p), \
                    sizeof(sub##ClassType), \
                    sub##_clinit, \
                    (void (*)(Object *)) deinit,\
    } \
    }; \
    \
    sub##ClassType *const sub##Class = sub##Clazz; \
    static void sub##_clinit() {
#endif
#define END } ;
#define IMPLEMENTS(type) struct type type
#define ALLOC(type) alloc((ClassType*)type)
#define EXTEND(sub,super) extend((ClassType*)sub,(ClassType*)super)
#endif //type_H
