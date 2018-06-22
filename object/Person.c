//
// Created by kenhuang on 2018/6/3.
//

#include "Person.h"
#include "foundation/interface/Runnable.h"
#include "foundation/type/Integer.h"
#include "foundation/type/Pointer.h"
#include "foundation/type/Float.h"

private_start(Person,Circle)
    float height;
private_end

static void draw(Person* self);
static Object* init(Object* self, va_list * parms){
    return  self;
}
static Object * deinit(Object * self){
    return  Class_getSuperClass(PersonClazz)->deinit(self);

}
static void draw(Person* self){
//    PersonClazz->superDraw((const Point *)self);
//    printf("person at %d,%d rad %lf age %d  height=%f\n",
//           ((Point*)self)->x,((Point*)self)-> y,  ((Circle*)self)-> radius,self->age,self->private->height);
}
static  void run(){
    for (int i = 0; i < 100; ++i) {
        printf("person:thread %d\n",i);
    }
}
/***********************************************************************************************************************
 *
 **********************************************************************************************************************/
Runnable* Person_conformToRunable(Person* self){
    Runnable* runnable =  Class_newInstance(RunnableClazz);
    runnable->_.object = (Object*)self;
    runnable->run = run;
    return runnable;
}
/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(Person,Circle,offsetof(Person,private), sizeof(__Person))

class_end
static void Class_init(Class1* class) {
    if (isReflectable) {
        Class_fillDeclaredFields(PersonClazz,
                                 "age",IntegerClazz,READABLE_WRITABLE_PUBLIC,
                                 "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                 "height",FloatClazz,READABLE_WRITABLE_PRIVATE,
                                 "");
        Class_fillDeclaredMethods(PersonClazz,
                                  init,READABLE_UNWRITABLE_PRIVATE,
                                  deinit,READABLE_UNWRITABLE_PRIVATE,
                                  run,READABLE_UNWRITABLE_PRIVATE,
                                  draw,READABLE_UNWRITABLE_PRIVATE,
                                  Person_conformToRunable,READABLE_UNWRITABLE_PUBLIC,
                                  NULL);
    }
}

static void Class_deinit(Class1* class) {

}

