#include "gtest/gtest.h"

extern "C"{
    struct Person* Person_init(Person* self,char* name);
    extern struct ClassType * const ObjectClass;
    extern struct ClassType * const Class;
    extern struct PersonClassType*PersonClass;
    extern struct ThreadClassType*ThreadClass;
    ClassType* extend(ClassType *sub,ClassType* super);
    struct Thread *Thread_init(Thread *self,struct Object* runnable);
    bool Thread_start(Thread* self);
    void* Thread_join(Thread* self);
    void Thread_exit(void* returnValue);
    void Thread_cancel(Thread* self);
    void * alloc (const ClassType * clazz);
    void multiDestroy(Object *self,...);
}

TEST(TEST, Thread) {
//    extend((ClassType*)PersonClass,ObjectClass);
//    Person* person = Person_init((Person*)alloc((ClassType*)PersonClass),"Tom");
//    Person* person1 = Person_init((Person*)alloc((ClassType*)PersonClass),"Jerry");
//    Thread* thread = Thread_init((Thread*)alloc((ClassType*)ThreadClass),( Object*)person);
//    Thread* thread1 = Thread_init((Thread*)alloc((ClassType*)ThreadClass),(Object*)person1);
//    Thread_start(thread);
//    Thread_start(thread1);
//    for (int i = 0; i < 30; ++i) {
//        printf("Main\n");
//    }
}