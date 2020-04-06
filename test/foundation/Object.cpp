#include <csetjmp>
#include "gtest/gtest.h"
#include "../include/option.h"

extern "C"{
    extern  struct ClassType * const ObjectClass;
    extern struct ClassType * const Class;
    extern struct StringClassType * const StringClass;
    extern ClassType*const BoolClass;
    extern ClassType *const ByteClass;
    extern struct ClassType * const Class;
    extern void * alloc (const ClassType * clazz);
    extern  ClassType* const ExceptionClass;
    extern ClassType *const IntegerClass;
    extern struct Integer *Integer_init(Integer *self, int value);
    extern int Integer_getValue(Integer* self);
    struct Exception* Exception_init(Exception* self);
    jmp_buf* Exception_getBuffer(Exception* self);
    struct Object* Object_init(Object *self);
    bool isKindOf(const Object* self, const  ClassType *clazz);
    bool isMemberOf(const Object* self, const ClassType *clazz);
    const ClassType * Object_getClass(const Object *self);
    void Object_addObserver(Object*self, Object*observer) ;
    void Object_removeObserver(Object *self, Object *observer);
    void Object_notifyObservers(Object*self, int dataEnum);
    void release(Object* self);
    void* retain(Object* self);
    void * cast(Object *self, const ClassType *clazz);
    void * alloc (const ClassType * clazz);
    bool isObject(void* pointer);
void destroy(Object *self);
void multiDestroy(Object *self,...);
struct String *String_init(String *self,char* value);
#if (defined(__OVERRIDE__) || defined(__POLYMORPHISM__)) && defined(__REFLECT__)
bool respondsTo(const Object *self, const struct String *name);
void *getValue(Object *self, struct String *name, struct Exception *exception);
void setValue(Object *self, struct String *name, void *newValue, struct Exception *exception);
#endif

ClassType* extend(ClassType *sub,ClassType* super);
void implements(ClassType *self, Object*interfaceInSelf,ClassType* interface,...);
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
char* String_getValue(String*self);
}

TEST(Object_init, Object) {

}

//TEST(isObject, Object) {
//    void* p = malloc(sizeof(int));
//    ASSERT_FALSE(isObject(p));
//    Object* o = Object_init((Object*)alloc(ObjectClass));
//    ASSERT_TRUE(isObject(o));
//}

TEST(Object_isKindOf, Object) {
    Object* o = Object_init((Object*)alloc(ObjectClass));
    ASSERT_TRUE(isKindOf(o,ObjectClass));
    struct Byte* b = (Byte*)alloc(ByteClass);
    ASSERT_TRUE(isKindOf((Object*)b,ObjectClass));
    ASSERT_FALSE(isKindOf((Object*)b,BoolClass));
}

TEST(Object_isMemberOf, Object) {
    struct Byte* b = (Byte*)alloc(ByteClass);
    ASSERT_TRUE(isMemberOf((Object*)b,ByteClass));
    ASSERT_FALSE(isMemberOf((Object*)b,ObjectClass));
}

TEST(Object_cast, Object) {
    struct Byte* b = (Byte*)alloc(ByteClass);
    Object* o = (Object*)cast((Object*)b,ObjectClass);
    ASSERT_TRUE(o != NULL);
}

TEST(Object_addObserver, Object){
    Object* o = Object_init((Object*)alloc(ObjectClass));
    Object* observer1 = Object_init((Object*)alloc(ObjectClass));
    Object* observer2 = Object_init((Object*)alloc(ObjectClass));
    Object_addObserver(o,observer1);
    Object_addObserver(o,observer2);
    Object_addObserver(o,observer1);
    Object_notifyObservers(o,20);
    Object_removeObserver(o,observer1);
    Object_removeObserver(o,observer2);
    Object_removeObserver(o,observer1);
}

TEST(Reflect, Object) {
    Object* o = Object_init((Object*)alloc(ObjectClass));
    String* s = String_init((String*)alloc((ClassType*)StringClass),"");
    ASSERT_FALSE(respondsTo(o,s));
    destroy((Object*)s);
//    s = String_init((String*)alloc((ClassType*)StringClass),"toString");
//    ASSERT_FALSE(respondsTo(o,s));
//    s = String_init((String*)alloc((ClassType*)StringClass),"Object_toString");
//    ASSERT_TRUE(respondsTo(o,s));
}

TEST(GETTER, Object) {
    Object* o = Object_init((Object*)alloc(ObjectClass));
    Exception* e = Exception_init((Exception*)alloc(ExceptionClass));
    String* s = String_init((String*)alloc((ClassType*)StringClass),"refCount");
    Integer* i = Integer_init((Integer*)alloc(IntegerClass),20);
    //这里无法使用自定义的try catch
    if(!setjmp(*Exception_getBuffer(e))){
        setValue(o,s,(Object*)i,e);
    }else{
        printf("One Exception catchec.");
    }
    String* valueString = String_init((String*)alloc((ClassType*)StringClass),"value");
    String* testString = (String*)alloc((ClassType*)StringClass);
    char* hhh = "xxxxxx";
    if(!setjmp(*Exception_getBuffer(e))){
        setValue((Object*)testString,valueString,&hhh,e);
        char* value = *(char**)getValue((Object*)testString,valueString,e);
        ASSERT_TRUE(strcmp(value,hhh) == 0);
    }else{
        printf("One Exception catchec.");
    }
}