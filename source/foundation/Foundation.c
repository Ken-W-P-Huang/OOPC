//
// Created by kenhuang on 2019-10-20.
//

#include "Foundation.h"
#include "type/SizeType.h"


void initEnvironment(){
    extend(ObjectClass,ObjectClass);
    EXTEND(ExceptionClass,ObjectClass);
    extend(Class,ObjectClass);
    EXTEND(LockClass,ObjectClass);
    //reflect
    EXTEND(AccessClass,ObjectClass);
#if (defined(__OVERRIDE__) || defined(__POLYMORPHISM__)) && defined(__REFLECT__)
    EXTEND(FieldClass,ObjectClass);
    EXTEND(MethodClass,ObjectClass);
#endif
    //type
    EXTEND(BoolClass,ObjectClass);
    EXTEND(ByteClass,ObjectClass);
    EXTEND(CharacterClass,ObjectClass);
    EXTEND(DoubleClass,ObjectClass);
    EXTEND(FloatClass,ObjectClass);
    EXTEND(IntegerClass,ObjectClass);
    EXTEND(LongClass,ObjectClass);
    EXTEND(PointerClass,ObjectClass);
    EXTEND(ShortClass,ObjectClass);
    EXTEND(SizeTypeClass,ObjectClass);
    EXTEND(StringClass,ObjectClass);
#if defined(__OVERRIDE__) || defined(__POLYMORPHISM__)
    //collection
    EXTEND(LinkedListClass,ObjectClass);
    EXTEND(LinkedListIteratorClass,ObjectClass);
    EXTEND(HashMapClass,ObjectClass);
    EXTEND(HashMapEntryClass,ObjectClass);
    EXTEND(HashMapEntryIteratorClass,ObjectClass);
#endif
    EXTEND(ArrayListClass,ObjectClass);
    EXTEND(ArrayListIteratorClass,ObjectClass);
    EXTEND(BinaryTreeClass,ObjectClass);
    //interface
    EXTEND(RunnableClass,ObjectClass);
    EXTEND(ComparableClass,ObjectClass);
    //thread
    EXTEND(ThreadClass,ObjectClass);
}
