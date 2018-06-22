//
// Created by kenhuang on 2018/6/2.
//

#ifndef  Object_H
#define  Object_H

#include <zconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
//todo 禁用assert 频繁调用影响性能
// #define NDEBUG
#include <assert.h>
#include <string.h>
#include <assert.h>
#include <wchar.h>
#include <stddef.h>
#include "type/type.h"
#include <execinfo.h>
#include "../debug/Debug.h"

/***********************************************************************************************************************
 *  1.为了避免不必要的警告信息和cast方法的使用，可以直接将其他非Object对象强制转换为Object对象。其他情况请使用Object_cast方法进行类型
 *    转换。
 *  2.Class实例相关的方法为类方法（面向对象的静态方法,不是C语言的static修饰的方法）。直接提供类方法进行调用，即不通过Class.XXX进行调用,
 *    虽然这可以实现，也更贴近面向对象的使用习惯，理由请参考第3 点。静态属性则直接声明为全局变量即可。如果开启反射，则对象的所有方法指针存
 *    放在_Class的Methods中，属性信息存放在_Class的Fields。
 *  3.所有的类的Class都是Class对象，不进行继承扩展，因为每扩展一个Class子类就会浪费父类方法个数 * 8byte(64位)，呈几何级增长，好处是可以
 *  使用结构体的特性快速定位静态方法指针；或者可以将所有方法放到另一个结构体中。
 *  4.init和deinit用于对象属性默认值设置和特殊内存空间的申请和释放/赋值，必须成对出现并且调用父类的init和deinit，否则可能造成内存泄漏。
 *  5.反射只支持GCC编译器 需要开启-rdynamic选项。其他编译器请自行重写fillDeclaredMethod相关代码。开启反射会检查方法名称是否和父类公有
 *   方法名称相同；自身私有属性名称和公有属性名称是否重名。
 *  6.浅拷贝先判断对象类型是否匹配，然后利用结构体的赋值特性直接赋值即可。
 **********************************************************************************************************************/
/* 类型声明 */
typedef struct Class1 Class1;
typedef struct Object Object;
extern bool isReflectable;
/* 类声明*/
struct Object {
    /* 私有属性*/
    struct _Object *  private;
    /* 公共属性*/
};

struct Class1 {
    Object _;			               /* 继承Object*/
    struct String * name;				/* 类名 */
    struct _Class * private;
    /* 必须重写的公有方法，用来对对象属性进行初始化*/
    Object * (* init)(Object *self, va_list *app);
    Object * (* deinit)(Object *self);
    /* 确认所有类都不需要以下方法则可以删掉 */
    struct String* (*toString)(Object* self);
    int (*hashCode)(Object* self);
    bool (*equals)(Object* self,Object* object);
    void (*update)(Object* self,Object*subject,int dataEnum);
    Object* (*clone)(Object* self);
} ;
#include "reflect/Field.h"
#include "reflect/Method.h"
#include "type/String.h"
#include "exception/Exception.h"
/* 公有方法*/
bool Object_isKindOf(const Object*_self, const struct Class1 *clazz);
bool Object_isMemberOf(const Object*_self, const struct Class1 *clazz);
void Object_destroy(Object *self);
const Class1 * Object_getClass(const Object *self);
void * Object_cast(void *self, const Class1 *clazz);
void Object_addObserver(Object*self, Object*observer) ;
void Object_removeObserver(Object *self, Object *observer);
void Object_notify(Object*self, int dataEnum);
bool Object_respondsTo(const Object *self, const String *name);
void *Object_getValue(Object *self, String *name,  Exception *exception);
void Object_setValue(Object *self, String *name, void *newValue,  Exception *exception);
void Object_release(Object* self);
void* Object_retain(Object* self);
/***********************************************************************************************************************
 *  Class类
 **********************************************************************************************************************/
extern const Class1 * ObjectClazz;
extern const Class1 * Clazz;
/* 静态方法 */
void Class_registerObjectClazz(bool reflectable);
void Class_unregisterObjectClazz();
const Class1 * Class_getSuperClass(const struct Class1 *self);
void* Class_newInstance(const Class1 * self, ...);
/* 反射,适用于没有使用__attribute__进行对象属性对齐的情况 */
void Class_fillDeclaredFields(const Class1 *self, ...);
void Class_fillDeclaredMethods(const Class1 *self, ...);
const Class1* Class_forName(String *className);
Field * Class_getDeclaredField(const Class1 *self, String *name);
Field* Class_getField(const Class1 *self, String *name);
Method * Class_getDeclaredMethod(const Class1 *self, String *name);
Method* Class_getMethod(const Class1 *self, const String *name);
void* Class_newInstanceWithFieldInfos(const Class1 *self, ...);
#endif //Object_H