#include <stdio.h>
#include "object/foundation/Environment.h"
#include "object/foundation/thread/Thread.h"
#include "object/foundation/thread/Lock.h"
#include "object/Singleton.h"
int testSingleton(){
    Singleton* singleton ;
    for (int i = 0; i < 100; ++i) {
        singleton = Singleton_getInstance();
        _DEBUG("%d the address of singleton is 0x%p \n",i,singleton);
    }
    return 10;
}
int main() {
    initEnvironment(true);
    int a;
    Point* point = (Point *) Class_newInstance(PointClazz);
    Point_draw(point);
    Circle* circle = (Circle*) Class_newInstance(CircleClazz);
    Point_draw(Object_cast(circle, (const Class1 *) PointClazz));
    printf("**************************************\n");
    Person* person = (Person *) Class_newInstance(PersonClazz);
    Point_draw(Object_cast(person, (const Class1 *) PointClazz) );
    Object_addObserver((Object *)person, (Object *)point);
    Object_addObserver((Object *)person, (Object *)circle);
    Object_notify((Object *)person, 1);
    printf("**************************************\n");
    Circle* circle1 = Circle_newWithXY(NULL, 100, 2000);
    Point_draw(Object_cast(circle1, PointClazz));
    Object_removeObserver((Object *)person, (Object *)point);
    Object_removeObserver((Object *)person, (Object *)circle);
    Object_removeObserver((Object *)person, (Object *)circle);
    Singleton* singleton = Singleton_getInstance();
    Field* field = Class_getDeclaredField(ObjectClazz,String_new(NULL,"private"));
    Method* method = Class_getDeclaredMethod(Clazz,String_new(NULL,"Class_getDeclaredField"));
    Exception* exception = Exception_new(NULL);
    double newValue = 5000;
    int x = 1000;
    float height = 33.0F;
    try(exception){
        Object_setValue((Object *)circle1,String_new(NULL,"radius"),&newValue,exception);
        Object_setValue((Object *)circle1,String_new(NULL,"x"),&x,exception);
        Object_setValue((Object *)person,String_new(NULL,"height"),&height,exception);
        x = *(int*)Object_getValue((Object *)circle1,String_new(NULL,"y"),exception);
        Object_setValue((Object *)person,String_new(NULL,"xxxxx"),&height,exception);
        _DEBUG("go on!");
    }catch{
        _DEBUG("setValue error!，errorType = %d,errormessage=%s \n",exception->type,exception->message);
    }finally{
        _DEBUG("setValue finished! \n");
    };
    const Class1* class1 = Class_forName(String_new(NULL,"Person"));
    const Class1* class2 = Class_forName(String_new(NULL,"Exception"));
    Method* method2 =  Class_getDeclaredMethod(class2,String_new(NULL,"Exception_getBuffer"));
    Object_destroy((Object*)method2);
    Object_destroy((Object*)exception);
    Object_destroy((Object*)circle1);
    Object_destroy((Object*)point);
    Object_destroy((Object*)circle);
    Object_destroy((Object*)person);
    Object_destroy((Object*)singleton);

    destroyEnvironment();
    return 0;
}