//
// Created by kenhuang on 2018/6/2.
//


#include "Point.h"
#include "foundation/type/Integer.h"


/***********************************************************************************************************************
 *  公共方法
 **********************************************************************************************************************/
void Point_draw (const Point * self) {
   // ((Class*) Object_getClass(self))-> draw(self);
}
Point* Point_newWithXY(void *_self, int x, int y){
    Point* self = _self ;
    if(!self){
        self = (Point *) Class_newInstance((Class1 *) PointClazz);
    }
    self->x = x;
    self->y = y;
    return self;
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static Object* init (Object* _self, va_list * parms) {
//    self->private = calloc(1, sizeof(struct _Point));
//    self = super((const Class*)PointClazz)->privateAlloc(self, parms);
//    self -> x = va_arg(* parms, int);
//    self -> y = va_arg(* parms, int);

    return Class_getSuperClass(PointClazz)->init(_self,parms);
}
static void update(Object* self, const Object * subject,const int dataEnum){
    printf("Point**** received notification %d \n",dataEnum);
}
static void draw(const Point * self){
    printf("Point at %d,%d\n", self -> x, self -> y);
}
static Object* deinit(Object* _self){
    return Class_getSuperClass(PointClazz)->deinit(_self);


}
class_start(Point,Object,0,0)
class_end
static void Class_init( Class1* class){
    if (isReflectable) {
        Class_fillDeclaredFields(PointClazz,
                                 "x",IntegerClazz,READABLE_WRITABLE_PUBLIC,
                                 "y",IntegerClazz,READABLE_WRITABLE_PUBLIC,
                                 "");
        Class_fillDeclaredMethods(PointClazz,
                                  init,READABLE_UNWRITABLE_PRIVATE,
                                  deinit,READABLE_UNWRITABLE_PRIVATE,
                                  update,READABLE_UNWRITABLE_PRIVATE,
                                  draw,READABLE_UNWRITABLE_PRIVATE,
                                  Point_newWithXY,READABLE_UNWRITABLE_PUBLIC,
                                  Point_draw,READABLE_UNWRITABLE_PUBLIC,
                                  NULL);
    }
}
static void Class_deinit( Class1* class){

}




