//
// Created by kenhuang on 2018/6/3.
//

#include "Circle.h"
#include "foundation/type/Double.h"


static void draw(Circle* self);
//static const void (* super_draw) (const struct Point * self);
static Object * init(Object * self, va_list * parms){
    //  self = super((const Class *)CircleClazz)->privateAlloc(self, parms);
    //   self->radius = va_arg(*parms, double);
    //   super_draw = ((Point*)self)->draw;
    //   ((Point*)self)->draw = draw;
    return  Class_getSuperClass( CircleClazz)->init(self,parms);
}
static Object * deinit(Object * self){
    return Class_getSuperClass( CircleClazz)->deinit(self);

}
static void update(Object * self, Object * subject, const int dataEnum){
    printf("Circle**** received notification %d\n",dataEnum);
}
static void draw(Circle* self){
    //  super_draw(self);
    printf("circle at %d,%d rad %lf\n",
           ((Point*)self)->x, self->_.y, self -> radius);

}
Circle* Circle_newWithXY(Circle *self, int x, int y){
    if(!self){
        self = (Circle *) Class_newInstance((Class1 *) CircleClazz);
    }
    self = (Circle*) Point_newWithXY(self, x, y);


    return self;
}
class_start(Circle,Point,0,0)
class_end
static void Class_init(Class1* class){
    class->update = update;
    if (isReflectable) {
        Class_fillDeclaredFields(CircleClazz,
                                 "radius",DoubleClazz,READABLE_WRITABLE_PUBLIC,
                                 "");
        Class_fillDeclaredMethods(CircleClazz,
                                  init,READABLE_UNWRITABLE_PRIVATE,
                                  deinit,READABLE_UNWRITABLE_PRIVATE,
                                  update,READABLE_UNWRITABLE_PRIVATE,
                                  draw,READABLE_UNWRITABLE_PRIVATE,
                                  Circle_newWithXY,READABLE_UNWRITABLE_PUBLIC,
                                  NULL);
    }
}
static void Class_deinit(Class1* class){

}







