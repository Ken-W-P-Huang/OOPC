//
// Created by kenhuang on 2018/6/22.
//

#ifndef  Object_H
#define  Object_H
#include "option.h"
#include "type.h"
#include "debug.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <printf.h>
#include <assert.h>
#include <stddef.h>
#include <signal.h>
/***********************************************************************************************************************
 *  1.为了避免不必要的警告信息和cast方法的使用，可以直接将其他非Object的对象强制转换为Object对象。某些函数返回void*而非Object*。
 *  其他情况请使用cast方法进行类型转换。建议消除所有警告信息,防止出现类型错误。
 *  2.Class实例相关的方法为类方法（面向对象的静态方法,不是C语言的static修饰的函数）。直接提供类方法进行调用，尽量不通过Class.XXX进行调
 *  用,虽然这可以实现，也更贴近面向对象的使用习惯，理由请参考第3点。静态属性则直接声明为全局变量即可。如果开启反射，则对象的所有方法指针存
 *    放在_Class的Methods中，属性信息存放在_Class的Fields。
 *  3.所有对象都有一个指针指向其Class对象。该类对象记录了创建对象所需要的基本信息，对象方法，静态属性，静态方法以及类初始化函数（
 *  类似于Java的静态代码块）。由于方法存放在类对象中，每扩展一个Class子类就会浪费父类方法个数 * 8byte(64位)的空间，呈几何级增长，
 *  好处是可以使用结构体的特性快速定位静态方法指针，实现多态；或者可以将所有方法放到另一个结构体中。
 *  4.init和deinit（由destroy函数自动调用）用于对象属性默认值设置和特殊内存空间的申请和释放/赋值，必须成对出现，否则可能造成内存泄漏。
 *  5.反射只支持GCC编译器 需要开启-rdynamic选项。其他编译器请自行重写fillDeclaredMethod相关代码。开启反射会检查方法名称是否和父类公有
 *   方法名称相同；自身私有属性名称和公有属性名称是否重名。
 *  6.ClassType意为Class。使用"Class"作为类名时，IDE无法在调试时显示其内容，故使用此名称!!!
 *  7.多态可以通过在对象定义方法指针实现，也可以将多态方法定义在类对象上。后者需调用复杂(xxxClass*)Object_getClass(xxx* o)->method
 *  最和谐的方法：使用Class创建元类对象，通过元类对象创建类对象（此时的初始化方法为静态方法），再通过类对象创建对象。
 *  8.将对象的大小信息设为私有的好处是不会混入类对象编程提示框中。但这些属性原本都是从类自身获取，公开并没有太大的关系，而且方便类的初始化。
 *  9.只要alloc函数存在，就可以根据类对象创建对象，故额外增加字段isAbstract用于判断该类是否是抽象的。
 *  10.实现的接口必须在一起，并且放在类的起始位置！！！Java9接口可以有私有方法和默认实现。这里可以通过在接口中定义c语言的static函数，并
 *  开放接口的init方法实现。
 *  11.由于类对象是单例，故类对象和其父类共享私有属性。这里有bug，纯正的面向对象语言的类对象属性只有一份拷贝，这里有多份（子类的数量+1）。
 **********************************************************************************************************************/

typedef enum {
    REAL = 0,
    ABSTRACT,
    INTERFACE
}ClassTypeEnum;

typedef struct {
    struct Object_P*  p;
} Object;

typedef struct ClassType{
    /* Class继承自Object */
    Object _;
    const char* const name;
    const size_t size ;
    /* 实例对象私有属性结构体的大小 */
    const size_t privateSize;
    /* 实例对象私有属性结构体在公有属性的位移 */
    const size_t privateOffset;
    const size_t classSize;
#if defined(__CLASS_ATTRIBUTE__)
    const ClassTypeEnum type;
    const bool isFinal;
#endif
    /* 静态初始化方法，在extend函数执行过程中被调用 */
    void (* clinit)();
    /* 对象析构方法，在destroy函数执行过程中被调用 */
    void (* const deinit)(Object *);
    struct ClassType_P* p;
#if defined(__OVERRIDE__) || defined(__POLYMORPHISM__)
    int (* const hashCode)(Object* self);
    bool (* const equals)(Object*self,Object* );
    char* (* const toString)(Object* );
    void (* const update)(Object* ,Object*,int);
    Object* (*clone)(Object *self);
    void (*addObserver)(Object *self, Object *observer) ;
    void (*removeObserver)(Object *self, Object *observer);
    void (*notifyObservers)(Object *self, int dataEnum);
#endif
} ClassType;

typedef ClassType ObjectClassType;
typedef ClassType InterfaceClassType;
typedef Object Interface;
extern  ClassType * const ObjectClass;
extern  ClassType * const InterfaceClass;
extern  ClassType * const Class;
#include "reflect/Access.h"
#include "reflect/Field.h"
#include "reflect/Method.h"
#include "exception/Exception.h"
#include "type/String.h"
#include "type/Bool.h"
#include "type/Byte.h"
#include "type/Character.h"
#include "type/Double.h"
#include "type/Float.h"
#include "type/Integer.h"
#include "type/Long.h"
#include "type/Short.h"
#include "type/Pointer.h"
#include "type/String.h"
#include "type/SizeType.h"
#include <execinfo.h>
#include "debug.h"
#include "collection/HashMap.h"
#include "type/Bool.h"
#include "thread/Lock.h"

Object* Object_init(Object *self);

#if defined(__OVERRIDE__) || defined(__POLYMORPHISM__)
char* Object_toString(Object *self);
bool Object_equals(Object *self, Object *obj);
int Object_hashCode(Object *self);
void Object_update(Object* self,Object* subject,int dataEnum);
Object* Object_clone(Object *self);
void Object_addObserver(Object *self, Object *observer) ;
void Object_removeObserver(Object *self, Object *observer);
void Object_notifyObservers(Object *self, int dataEnum);
#endif

/* 以下是和Class/对象有关的工具函数，不应当被重写 */
void * alloc(const ClassType *clazz);
void* release(Object *self);
void* retain(Object *self);
void * cast(Object *self, const ClassType *clazz);
void* getInterface(Object *self,ClassType*interface);
bool isKindOf(const Object *self, const ClassType *clazz);
bool isMemberOf(const Object *self, const ClassType *clazz);
const ClassType * getClass(const Object *self);


/* 释放内存函数，delete是C++关键字，故使用destroy */
void destroy(Object *self);
void multiDestroy(Object *self,...);
/* 此方法的实现无法断点调试 */
bool isObject(void* pointer);

#if (defined(__OVERRIDE__) || defined(__POLYMORPHISM__)) && defined(__REFLECT__)
bool respondsTo(const Object *self, const struct String *name);
void *getValue(Object *self, struct String *name, struct Exception *exception);
void setValue(Object *self, struct String *name, void *newValue, struct Exception *exception);
#endif

ClassType* extend(ClassType *sub,ClassType* super);
void implements(ClassType *self, Object*interfaceAddressInSelf,ClassType* interfaceClass,...);
const ClassType * Class_getSuperClass(const ClassType *self);
/* 启用多态时，往类对象中批量写入函数以作为方法。非必需，也可以使用赋值语句。 */
void Class_addFunctions(ClassType *self, ...);
/* 反射,适用于没有使用__attribute__进行对象属性对齐的情况 */
void Class_fillDeclaredFields(const ClassType *self, ...);
void Class_fillDeclaredMethods(const ClassType *self, ...);

#if (defined(__OVERRIDE__) || defined(__POLYMORPHISM__)) && defined(__REFLECT__)
const ClassType* Class_forName(struct String *className);
struct Field* Class_getDeclaredField(const ClassType *self, struct String *name);
Field* Class_getField(const ClassType *self,struct String *name);
struct Method * Class_getDeclaredMethod(const ClassType *self,struct String *name);
struct Method* Class_getMethod(const ClassType *self, const struct String *name);
void* Class_newInstanceWithFields(const ClassType *self, ...);
#endif

#endif //Object_H