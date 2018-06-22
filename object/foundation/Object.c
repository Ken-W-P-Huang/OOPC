//
// Created by kenhuang on 2018/6/2.
//


#include "collection/HashMap.h"
#include "type/Bool.h"
#include "type/Byte.h"
#include "type/Character.h"
#include "type/Double.h"
#include "type/Float.h"
#include "type/Long.h"
#include "type/Pointer.h"
#include "type/Short.h"
#include "type/Integer.h"

#include "collection/ArrayList.h"
#include "thread/Thread.h"

/***********************************************************************************************************************
 *  Object类
 **********************************************************************************************************************/
/* 类型声明 */
typedef struct _Class _Class;

/* 私有属性 */
struct _Object{
    const struct Class1 * class;	       /* 指向对象的类模版 */
    unsigned  int refCount;                /* 引用计数 */
    LinkedList* observerList;              /* 观察者链表 */
};
struct _Class{
    const struct Class1 * super;		   /* 父类 */
    size_t size;					       /* 实例对象的大小 */
    size_t privateSize;                    /* 实例对象私有属性的大小 */
    size_t privateOffset;                  /* 实例对象私有属性在公有属性的位移 */
 //   size_t classSize;                    /* 类的大小 */
    HashMap* subClasses;                   /* 子类表，用于反射*/
    HashMap* fields;                       /* 属性*/
    HashMap* methods;                      /* 方法*/
};
const Class1 * ObjectClazz;
const Class1 * Clazz ;
bool isReflectable;
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
/**
 * Object层级的私有属性空间分配已经在Class_alloc时完成，故不做任何操作，直接返回，从而可以让子类可以不必实现alloc方法。
 * @param _self 必要时需要被强制转换成可用的类型self
 * @param app
 * @return
 */
static Object* init(Object *self, va_list *parms) {
    return self;
}
/**
 * Object层级的私有属性空间释放已经在Class_dealloc时完成，故不做任何操作，直接返回，从而可以让子类可以不必实现dealloc方法。
 * @param self
 * @return
 */
static Object* deinit(Object *self) {
    return self;
}
static void deallocSpace(Object *self){
    const Class1* super = Object_getClass(self);
    _DEBUG("**************************************\n");
    _DEBUG("start free space  for class %s",String_getValue(super->name) );
    void** pointer;
    if(super->private->privateSize){
        _DEBUG("free  %ld byte for class %s private %p",super->private -> privateSize ,
               String_getValue(super->name),super);
        pointer = (void**)((byte*)self + super->private->privateOffset);
        free(*pointer);

    }else{
        _DEBUG("ignore free for class %s private ",String_getValue(super->name));
    }

    /**
     * 最后一个释放的对象是Clazz需要单独销毁其作为Object 对象时的私有内存
     */
    if(self == (Object*)Clazz){
        free(self->private);
    } else{
        /**
         * 避免重复释放内存
         */
        while(super != ObjectClazz ){
            super = super->private->super;
            if(super->private->privateSize){
                char * a = String_getValue(super->name);
//                printf("aa%s",a);
//                _DEBUG("free  %ld byte for class %s private %p",super->private -> privateSize ,
//                           String_getValue(super->name),super);
                pointer =(void**)((byte*)self + super->private->privateOffset);
                free(*pointer);
            }else{
                _DEBUG("ignore free for class %s private ",String_getValue(super->name));
            }
        }
    }
    free(self);
    _DEBUG("**************************************\n");
}
static void update(Object* self,Object* subject,int dataEnum){
    printf("update from Object\n");
}
static String * toString(Object *self){
    return self->private->class->name;
}
static bool equals(Object* self,Object* obj){
    return self == obj;
}
static int hashCode(Object* self){
    return (long)self;
}
static void* clone(const Object*self){
    const Class1* class = Object_getClass(self);
    void* pointer = calloc(1,class->private->size);
    memcpy(pointer,self,class->private->size);
    return pointer;
}
/***********************************************************************************************************************
 *  公共方法
 **********************************************************************************************************************/
void* Object_retain(Object* self){
    if(self){
        self->private->refCount++;
        _DEBUG("an instance of %s is retained,refCount=%u",String_getValue(Object_getClass(self)->name),
               self->private->refCount);

    }
    return self;
}
void Object_release(Object* self){
    _DEBUG("an instance of %s is released,refCount=%u",String_getValue(Object_getClass(self)->name),
           self->private->refCount - 1);
    if(self && 0 == --self->private->refCount){
        Object_destroy(self);
        _DEBUG("an instance of %s is freed",String_getValue(Object_getClass(self)->name));
    }
}
/**
 * 通用get方法，Access位域属性的name为"access"
 * @param self
 * @param name
 * @return
 */
void *Object_getValue(Object *self, String *name, Exception *exception) {
    const Class1* class = Object_getClass(self) ;
    const Class1* super = class;
    void** valuePointer = NULL ;
    Field* field = Class_getDeclaredField(super,name);
    while(!field  && super != ObjectClazz){
        super = Class_getSuperClass(super);
        field = Class_getDeclaredField(super,name);
    }
    if(field){
        Access* access = Field_getAccess(field);
        if(Access_getReadable(access)){
            const Class1* type = Field_getType(field);
            size_t offset = Field_getOffset(field);
            if(Access_getIsPrivate(access)){
                valuePointer = (void **)((byte *)self + class->private->privateOffset);
                valuePointer =  *valuePointer + offset;
            }else{
                valuePointer = (void **)((byte *)self + offset)  ;
            }
        }else{
            exception->type = FIELD_NO_ACCESS;
            throw(exception);
        }
    }else{
        exception->type = FIELD_NOT_FOUND;
        throw(exception);
    }
    return valuePointer;
}
/**
 * 将name指定的对象属性设置为newValue值，Access位域属性的name为"access"
 * @param self
 * @param name
 * @param newValue
 * @param exception
 */
void Object_setValue(Object *self, String *name, void *newValue, Exception *exception) {
    const Class1* class = Object_getClass(self) ;
    const Class1* super = class;
    Field* field = Class_getDeclaredField(super,name);
    while(!field  && super != ObjectClazz){
        super = Class_getSuperClass(super);
        field = Class_getDeclaredField(super,name);
    }
    if(field ){
        Access* access = Field_getAccess(field);
        if(Access_getWritable(access)){
            const Class1* type = Field_getType(field);
            void** destination;
            size_t offset = Field_getOffset(field);
            size_t length = Field_getLength(field);
            if(Access_getIsPrivate(access)){
                destination = (void **)((byte *)self +  class->private->privateOffset);
                destination = *destination+ offset;
            }else{
                destination = (void **)((byte *)self + offset);
            }
            memcpy(destination,newValue,length);
        }else{
            exception->type = FIELD_NO_ACCESS;
            throw(exception);
        }
    }else{
        exception->type = FIELD_NOT_FOUND;
        throw(exception);
    }
}
/**
 * 返回指定Object的Class，可以转换为指定类型的Class。
 * @param self
 * @return
 */
const Class1 * Object_getClass(const Object *self) {
    assert(self && self->private->class);
    return self->private->class;
}

/**
 * 销毁对象
 * @param self
 */
void Object_destroy(Object *self) {
    if(self->private->observerList){
        assert(LinkedList_getSize(self->private->observerList) == 0);
        Object_destroy((Object *)self->private->observerList);
    }
    if (self){
        const Class1 * class = Object_getClass(self);
        assert(class->deinit);
        class -> deinit(self);
        deallocSpace(self);
    }
}

/**
 *  判断对象是否属于指定类
 * @param self
 * @param class
 * @return
 */
bool Object_isMemberOf(const Object*_self, const struct Class1 *class){
    const Object* self = _self;
    return self && self->private->class == class;
}
/**
 *  判断对象是否属于指定类的子类
 * @param self
 * @param clazz
 * @return
 */
bool Object_isKindOf(const Object*_self, const struct Class1 *clazz){
    const Object* self = _self;
    if(self){
        Class1* classNode = (Class1 *)self->private->class;
        if(clazz != ObjectClazz ){
            while (classNode != clazz){
                if(classNode != ObjectClazz){
                    classNode = (Class1 *) Class_getSuperClass((const Class1 *) classNode);
                }else{
                    return  false;
                }
            }
        }
        return  true;
    }
    return  false;
}
void * Object_cast(void *self, const Class1 *clazz){
        assert(Object_isKindOf(self, clazz));
        return self;
}

bool Object_respondsTo(const Object *self, const String *name){
    if (name ) {
        const  Class1 * class = Object_getClass(self);
        return Class_getMethod(class,name) != NULL;
    }
    return false;
}
// 观察者模式
/**
 *  将observer加入subject中的observer链表
 * @param self
 * @param observer
 * @return
 */
void Object_addObserver(Object*_self, Object*observer) {
    const Object* self = _self;
    if(!self->private->observerList){
        self->private->observerList = (LinkedList *) Class_newInstance((const Class1 *) LinkedListClazz);
    }
    LinkedList_add(self->private->observerList,observer);
}
/**
 * 将observer踢出subject中的observer链表
 * @param self
 * @param observer
 * @return
 */
void Object_removeObserver(Object *_self, Object *observer) {
    const Object* self = _self;
    LinkedList_remove(self->private->observerList,observer);
}
/**
 * 通知观察者
 * @param pObject
 */
void Object_notify(Object*self, int dataEnum){
    Object* observer;
    const Class1* class;
    LinkedList* list = self->private->observerList;
    unsigned  size = LinkedList_getSize(list);
    for (int i = 0; i < size; ++i) {
        observer = LinkedList_get(list,i);
        class = Object_getClass(observer);
        (*class->update)(observer,self,dataEnum);
    }
}

/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
/* 私有静态方法*/
static Object* allocSpace(const Class1 *class){
    _DEBUG("**************************************\n");
    _DEBUG("start space allocation for class %s",String_getValue(class->name));
    Object* object = calloc(1, class->private -> size);
    byte**pointer;
    if(class->private -> privateSize > 0){
        pointer = (byte**)((byte*)object + class->private->privateOffset);
        *pointer = calloc(1, class->private -> privateSize);
        _DEBUG("allocate  %ld byte for class %s %p private",class->private -> privateSize ,
               String_getValue(class->name),class);
    }else{
        _DEBUG("ignore allocation for class %s private",String_getValue(class->name));
    }
    const Class1* super = class;
    while(super != ObjectClazz){
        super = super->private->super;
        if(super->private -> privateSize > 0){
            pointer = (byte**)((byte*)object + super->private->privateOffset);
            *pointer = calloc(1, super->private -> privateSize);
            _DEBUG("allocate  %ld byte for class %s private %p",super->private -> privateSize ,
                   String_getValue(super->name),super);
        }else{
            _DEBUG("ignore allocation for class %s private ",String_getValue(super->name));
        }
    }
    _DEBUG("end allocation for class %s",String_getValue(class->name));
    _DEBUG("**************************************\n");
    return object;
}
/**
 * Class_forName调用,遍历整个Class继承树，查找匹配的Class
 * @param className
 * @return
 */
static const Class1* searchClassInClassTree(String* className){
    Class1* class,*super;
    LinkedList* stack = LinkedList_new(NULL);
    LinkedList_push(stack,(Object*)ObjectClazz);
    _Class* private;
    while(LinkedList_getSize(stack)){
        super = (Class1*)LinkedList_pop(stack);
        private = super->private;
        class = (Class1*)HashMap_get(private->subClasses,(Object*)className);
        if(class){
            return class;
        }else{
            if(HashMap_getSize(private->subClasses) > 0){
                HashMapEntryIterator* iterator = HashMap_getIterator(private->subClasses);
                while(HashMapEntryIterator_hasNext(iterator)){
                    LinkedList_push(stack,HashMapEntryIterator_next(iterator)->value);
                }
                Object_destroy((Object*)iterator);
            }
        }
    }
    Object_destroy((Object*)stack);
    return NULL;
}
/**
 * Class_forName调用
 * @param className
 * @return
 */
//todo 机器码注入/动态链接库？
static const Class1* loadClass(String* className){
    Class1* class = NULL;
    return class;
}
/**
 * 为initObjectClazz方法创建ObjectClazz和Clazz。
 * @param name
 * @param size
 * @param privateOffset
 * @param init
 * @param deinit
 * @return
 */
static Class1 *newClazz( size_t size, size_t privateSize,
                        size_t privateOffset,Object *(*init)(Object *, va_list *) ,
                        Object *(*deinit)(Object *),
                        void (*update)(Object* ,Object* ,int ),
                        String* (*toString)(Object* ),
                        bool (*equals)(Object* ,Object* ),
                        int (*hashCode)(Object* )) {
    struct _Class* _private = calloc(1, sizeof(struct _Class));
    _private->size = size;
    _private->privateOffset = privateOffset;
    _private->privateSize = privateSize;
    Class1* clazz = calloc(1, sizeof(Class1));
    clazz->private = _private;
    clazz->init = init;
    clazz->deinit = deinit;
    clazz->update =update;
    clazz->equals = equals;
    clazz->toString = toString;
    clazz->hashCode = hashCode;
    ((Object*)clazz)->private = calloc(1,sizeof(struct _Object));
    return clazz;
}

static Class1 * class_init(Class1 *self, va_list *parms) {
    Function function;
    const static size_t offset = offsetof(Class1,init);
    const static size_t classSize = sizeof(Class1);
    char* name = va_arg(* parms, char *);
    _Class* private = self -> private;
    private ->super = va_arg(* parms, Class1 *);
    private->size = va_arg(* parms, size_t);
    private->privateSize = va_arg(*parms,size_t);
    private->privateOffset = va_arg(*parms,size_t);
    assert(private -> super);
    /**
     * 将父类的方法复制给子类，实现继承。
     */
    memcpy((byte *) self + offset,
           (byte *) private -> super + offset,
           classSize - offset);
    self -> init = va_arg(*parms,  Object * (* )(Object *self, va_list *app));
    self -> deinit = va_arg(*parms, Object * (* )(Object *self));
    if(StringClazz){
        self ->name = String_new(NULL,name);
    }
    if(isReflectable && HashMapClazz){
        private->fields = HashMap_new(NULL);
        private->methods = HashMap_new(NULL);
        private->subClasses = HashMap_new(NULL);
        if(LinkedListClazz && LinkedListIteratorClazz && LinkedListNodeClazz){
            HashMap_put(private->super->private->subClasses,(Object*)self ->name,(Object*)self);
        }
    }
    return self;
}
static Class1 * class_deinit(Class1 *self) {
    _Class* private = self->private;
    /* StringClazz，ObjectClazz和Clazz在HashMapClazz 和LinkedListClazz释放之后释放*/
    if(private && HashMapClazz && LinkedListClazz){
        if(private->subClasses){
            HashMap_clear(private->subClasses);
        }
        if(private->fields){
            HashMap_clear(private->fields);
        }

        if(private->methods){
            HashMap_clear(private->methods);
        }
    }
    if(self->name){
        printf("warning:%s will be destroyed! \n", String_getValue(self->name));
        Object_destroy((Object*)self->name);
    }
    return self;
}
//todo 改为每个Class对象的getDataSize方法，避免多重if？
static size_t getDataSize(const Class1 *self){
    size_t size;
    if(self == IntegerClazz){
        size = sizeof(int);
    }else if(self == BoolClazz){
        size = sizeof(bool);
    }else if(self == ByteTClazz){
        size = sizeof(byte);
    }else if(self == CharacterClazz){
        size = sizeof(char);
    }else if(self == DoubleClazz){
        size = sizeof(double);
    }else if (self == FloatClazz){
        size = sizeof(float);
    }else if(self == LongClazz){
        size = sizeof(long);
    }else if(self == PointerClazz){
        size = sizeof(void*);
    }else if(self == ShortClazz){
        size = sizeof(short);
    }else{
        size =  self->private->size;
    }
    return size;
}



/**
 * 从指定链表中获取指定名称的Method
 * @param methodMap
 * @param name
 * @param parameterTypes
 * @return
 */
static Method* getMethod(HashMap *methodMap, const String *name, Class1 **parameterTypes){
    return (Method*)HashMap_get(methodMap,(Object*)name);
}
static Field * getField(HashMap *map, String *name){
    Object* field = HashMap_get(map,(Object*)name);
    if(field){
        return Object_cast(field,FieldClazz);
    }
    return NULL;

}
static String * getFunctionName(Function method){
    void *buffer[1] = {method};
    String* methodName;
    char **strings = backtrace_symbols(buffer, 1);
    if (strings == NULL) {
        perror("backtrace_symbols");
    } else {
        char* name = strings[0];
        methodName =String_newWithOffsetLength(NULL,name,59,strlen(name)-63);
        _DEBUG("%s",strings[0]);
        free(strings);
    }
    return methodName;
}
static void createMethodFieldStorageFor(const Class1* clazz){
    clazz->private->subClasses = HashMap_new(NULL);
    clazz->private->fields = HashMap_new(NULL);
    clazz->private->methods =HashMap_new(NULL);
}
static void removeSubClasseMethodFieldStorageFor(const Class1 *clazz){
    _Class* private = clazz->private;
    Object_destroy((Object *)private->subClasses);
    private->subClasses = NULL;
    //todo subClasses中key值会被释放，fields和methods的key值没有被释放。
    HashMap_clearAllKeys(private->fields);
    Object_destroy((Object *)private->fields);
    private->fields = NULL;
    HashMap_clearAllKeys(private->methods);
    Object_destroy((Object *)private->methods );
    private->methods = NULL;
}
static void finalizeUnregisteration(){
    if(Clazz){
        if(isReflectable){
            removeSubClasseMethodFieldStorageFor(LinkedListClazz);
            removeSubClasseMethodFieldStorageFor(StringClazz);
            removeSubClasseMethodFieldStorageFor(HashMapClazz);
            removeSubClasseMethodFieldStorageFor(FieldClazz);
            removeSubClasseMethodFieldStorageFor(MethodClazz);
            removeSubClasseMethodFieldStorageFor(AccessClazz);
            removeSubClasseMethodFieldStorageFor(Clazz);
            removeSubClasseMethodFieldStorageFor(ObjectClazz);
            Class_unregisterFieldClazz();
        }
        Class_unregisterLinkedListClazz();
        Class_unregisterHashMapClazz();
        Object_destroy((Object*)Clazz->name);
        ( (Class1*)Clazz)->name = NULL;
        Object_destroy((Object*)ObjectClazz->name);
        ( (Class1*)ObjectClazz)->name = NULL;
        Class_unregisterStringClazz();
        Object_destroy((Object*)ObjectClazz);
        ObjectClazz = NULL;
        Object_destroy((Object*)Clazz);
        Clazz = NULL;
    }
}
static void registerObjectClazz(){
        /* 创建Clazz和ObjectClazz */
        Class1 * tempObjectClazz = newClazz( sizeof(Object), sizeof(struct _Object),offsetof(Object,private),
                                             init, deinit,update,toString,equals,hashCode);
        Class1 * tempClazz = newClazz(sizeof(Class1), sizeof(struct _Class), offsetof(Class1,private),
                                      (Object *(*)(Object *, va_list *)) class_init,
                                      (Object *(*)(Object *)) class_deinit,update,toString,equals,hashCode);
        tempObjectClazz->_.private->class= tempClazz;
        tempObjectClazz->private->super = tempObjectClazz;
        tempClazz->_.private->class = tempClazz;
        tempClazz->private->super = tempObjectClazz;
        ObjectClazz = tempObjectClazz;
        Clazz = tempClazz;
        /* 创建StringClazz */
        Class_registerStringClazz();
        tempClazz->name = String_new(NULL,"Class");
        tempObjectClazz->name = String_new(NULL,"Object");
        ((Class1*)StringClazz)->name = String_new(NULL,"String");
        Class_registerHashMapClazz();
        Class_registerLinkedListClazz();
        if(isReflectable){
            createMethodFieldStorageFor(ObjectClazz);
            createMethodFieldStorageFor(Clazz);
            createMethodFieldStorageFor(StringClazz);
            createMethodFieldStorageFor(HashMapClazz);
            Class_registerFieldClazz();
            HashMap* subClasses = ObjectClazz->private->subClasses;
            HashMap_put(subClasses,(Object*)Clazz->name,(Object*)Clazz);
            HashMap_put(subClasses,(Object*)StringClazz->name,(Object*)StringClazz);
            HashMap_put(subClasses,(Object*)LinkedListClazz->name,(Object*)LinkedListClazz);
            HashMap_put(subClasses,(Object*)LinkedListNodeClazz->name,(Object*)LinkedListNodeClazz);
            HashMap_put(subClasses,(Object*)LinkedListIteratorClazz->name,(Object*)LinkedListIteratorClazz);
            HashMap_put(subClasses,(Object*)HashMapClazz->name,(Object*)HashMapClazz);
        }
        Class_registerBoolClazz();
        Class_registerByteTClazz();
        Class_registerCharacterClazz();
        Class_registerDoubleClazz();
        Class_registerFloatClazz();
        Class_registerIntegerClazz();
        Class_registerLongClazz();
        Class_registerShortClazz();
        Class_registerPointerClazz();
        Class_registerArrayListClazz();
        Class_registerExceptionClazz();
        Class_registerInterfaceClazz();
        Class_registerThreadClazz();
        if(isReflectable){
            Class_fillDeclaredFields(ObjectClazz,
                                     "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                     "class",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                                     "refCount",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                                     "observerList",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                                     "");
            Class_fillDeclaredMethods(ObjectClazz,init,READABLE_UNWRITABLE_PRIVATE,
                                      deinit,READABLE_UNWRITABLE_PRIVATE,
                                      deallocSpace,READABLE_UNWRITABLE_PRIVATE,
                                      update,READABLE_UNWRITABLE_PRIVATE,
                                      toString,READABLE_UNWRITABLE_PRIVATE,
                                      equals,READABLE_UNWRITABLE_PRIVATE,
                                      hashCode,READABLE_UNWRITABLE_PRIVATE,
                                      Object_retain,READABLE_UNWRITABLE_PUBLIC,
                                      Object_release,READABLE_UNWRITABLE_PUBLIC,
                                      Object_getValue,READABLE_UNWRITABLE_PUBLIC,
                                      Object_setValue,READABLE_UNWRITABLE_PUBLIC,
                                      Object_getClass,READABLE_UNWRITABLE_PUBLIC,
                                      Object_destroy,READABLE_UNWRITABLE_PUBLIC,
                                      Object_isMemberOf,READABLE_UNWRITABLE_PUBLIC,
                                      Object_isKindOf,READABLE_UNWRITABLE_PUBLIC,
                                      Object_cast,READABLE_UNWRITABLE_PUBLIC,
                                      Object_respondsTo,READABLE_UNWRITABLE_PUBLIC,
                                      Object_addObserver,READABLE_UNWRITABLE_PUBLIC,
                                      Object_removeObserver,READABLE_UNWRITABLE_PUBLIC,
                                      Object_notify,READABLE_UNWRITABLE_PUBLIC,
                                      NULL);
            Class_fillDeclaredFields(Clazz,"_",ObjectClazz,READABLE_UNWRITABLE_PUBLIC,
                                     "name",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                     "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                     "init",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                     "deinit",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                     "toString",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                     "hashCode",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                     "equals",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                     "update",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                     "super",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                                     "size",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                                     "privateSize",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                                     "privateOffset",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                                     "subClasses",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                                     "fields",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                                     "methods",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                                     "");
            Class_fillDeclaredMethods(Clazz,allocSpace,READABLE_UNWRITABLE_PRIVATE,
                                      newClazz,READABLE_UNWRITABLE_PRIVATE,
                                      class_init,READABLE_UNWRITABLE_PRIVATE,
                                      class_deinit,READABLE_UNWRITABLE_PRIVATE,
                                      getDataSize,READABLE_UNWRITABLE_PRIVATE,
                                      getMethod,READABLE_UNWRITABLE_PRIVATE,
                                      getField,READABLE_UNWRITABLE_PRIVATE,
                                      Class_newInstance,READABLE_UNWRITABLE_PUBLIC,
                                      Class_getSuperClass,READABLE_UNWRITABLE_PUBLIC,
                                      Class_forName,READABLE_UNWRITABLE_PUBLIC,
                                      Class_getDeclaredField,READABLE_UNWRITABLE_PUBLIC,
                                      Class_getDeclaredMethod,READABLE_UNWRITABLE_PUBLIC,
                                      Class_getMethod,READABLE_UNWRITABLE_PUBLIC,
                                      Class_newInstanceWithFieldInfos,READABLE_UNWRITABLE_PUBLIC,
                                      NULL);
            FieldClazz_fillFieldsMethods();
            HashMapClazz_fillFieldsMethods();
            LinkedListClazz_fillFieldsMethods();
            BoolClazz_fillFieldsMethods();
            ByteTClazz_fillFieldsMethods();
            CharacterClazz_fillFieldsMethods();
            DoubleClazz_fillFieldsMethods();
            FloatClazz_fillFieldsMethods();
            IntegerClazz_fillFieldsMethods();
            LongClazz_fillFieldsMethods();
            PointerClazz_fillFieldsMethods();
            ShortClazz_fillFieldsMethods();
            StringClazz_fillFieldsMethods();
        }
}
static void unregisterObjectClazz(){
    Class_unregisterArrayListClazz();
    Class_unregisterExceptionClazz();
    Class_unregisterInterfaceClazz();
    Class_unregisterThreadClazz();
    Class_unregisterBoolClazz();
    Class_unregisterByteTClazz();
    Class_unregisterPointerClazz();
    Class_unregisterCharacterClazz();
    Class_unregisterDoubleClazz();
    Class_unregisterFloatClazz();
    Class_unregisterIntegerClazz();
    Class_unregisterLongClazz();
    Class_unregisterShortClazz();
    finalizeUnregisteration();
}
/*  公有静态方法 */
/**
 * 为Object类和Class类构建Class实例
 */

void Class_registerObjectClazz(bool reflectable){
    isReflectable = reflectable;
    static pthread_once_t once =  PTHREAD_ONCE_INIT;
    pthread_once(&once,registerObjectClazz);
}

/**
 * 销毁ObjectClazz和Clazz实例
 */
void Class_unregisterObjectClazz(){
    static pthread_once_t once =  PTHREAD_ONCE_INIT;
    pthread_once(&once,unregisterObjectClazz);
}

/**
 * 使用Class实例对象来创建Object对象，
 * @param _class
 * @param ...
 * @return
 */
void * Class_newInstance(const Class1 *class, ...) {
    Object * object;
    va_list parms;
    object = allocSpace(class);
    assert(object);
    object->private->refCount = 1;
    object->private->class = class;
    va_start(parms, class);
    assert(class -> init);
    object = class -> init(object, &parms);
    va_end(parms);
    return object;
}
const Class1 * Class_getSuperClass(const Class1 *self){
    assert(self && self->private->super);
    return self->private->super;
}
const Class1* Class_forName(String *className){
    const Class1* class = searchClassInClassTree(className);
    if(!class){
        class = loadClass(className);
    }
    return  class;
}

/**
 * 获取当前类定义的指定属性不包含父类属性
 * @param self
 * @param name
 * @return
 */
Field * Class_getDeclaredField(const Class1 *self, String *name) {
    Field* field = NULL;
    field = getField(self->private->fields,name);
    return  field;
}
/**
 * 从当前类及其父类获取指定公共属性。
 * @param self
 * @param name
 * @return
 */
Field* Class_getField(const Class1 *self, String *name) {
    Field* field;
    const Class1* super = self;
    field = getField(super->private->fields, name);
    while(super != ObjectClazz && (!field || Access_getIsPrivate(Field_getAccess(field)))){
        super = super->private->super;
        field = getField(super->private->fields, name);
    }
    return field;
}
/**
 * 从本类所有方法中获取name指定的方法
 * @param self
 * @param name
 * @param isPrivate
 * @return
 */
Method * Class_getDeclaredMethod(const Class1 *self, String *name) {
    Method *selector = NULL;
    selector = getMethod(self->private->methods, name, NULL);
    return selector;
}
/**
 * 从当前类及其父类的公共方法中获取指定公共方法。
 * @param self
 * @param name
 * @param isPrivate
 * @return
 */
Method* Class_getMethod(const Class1 *self, const String *name) {
    Method *selector;
    const Class1* super = self;
    selector = getMethod(super->private->methods, name, NULL);
    while(super != ObjectClazz && (!selector || Access_getIsPrivate(Method_getAccess(selector)))){
        super = super->private->super;
        selector = getMethod(super->private->methods, name, NULL);
    }
    return selector;
}

/**
 * 往Class中填充对象属性类型。
 * @param self
 * @param ... 次序为属性名，属性类型，属性访问权限Access...。属性类型必须为Class实例，例如int对应的类型为IntegerClass，指针对应的类型
 * 为Pointer。必须按公有属性到私有属性依次填充。
 */
void Class_fillDeclaredFields(const Class1 *self, ...){
    va_list  parms;
    char* nameArray;
    String* name;
    Class1* type;
    size_t offset ;
    size_t length = 0;
    size_t remainder =0;
    Access* access;
    bool privateEncountered = false;
    /* 从父类"_"之后开始计算 */
    offset =self->private->super->private->size;
    va_start(parms,self);
    while ((nameArray = va_arg(parms,char*)) && nameArray[0] != '\0') {
        /* 检查属性是否已经定义 */
        name = String_new(NULL,nameArray);
        assert(!Class_getDeclaredField(self,name));
        type = va_arg(parms,Class1*);
        access = va_arg(parms,Access*);
        /*不检查属性名称是否和父类公有属性相同，Java可以相同，swift不可以。但这里父类和子类均有属性名"_",只检查类的属性名称是否相同*/
//        if(!access.ISPRIVATE){
//            assert(!Class_getField(self->private->super,name));
//        }
        if(!privateEncountered && Access_getIsPrivate(access)){
            privateEncountered = true;
            offset = 0;
        }
        length = getDataSize(type);
        remainder = offset % length;
        if (remainder != 0){
            offset += remainder;
        }
        Field * field = Field_new(NULL, self, name, type, offset, length, access);
        HashMap_put(self->private->fields,(Object*)name,(Object*)field);
        offset += length;
    }
    va_end(parms);
}
/**
 * 往Class中填充对象方法。
 * @param self
 * @param ...  次序为方法指针，方法访问权限Access。
 */
void Class_fillDeclaredMethods(const Class1 *self, ...){
    va_list  parms;
    String* name;
    Function function;
    Access* access;
    va_start(parms,self);
    while ((function = va_arg(parms,Function))) {
        name = getFunctionName(function);
        /* 检查方法名是否已经定义，防止重复插入 */
        assert(!Class_getDeclaredMethod(self,name));
        access = va_arg(parms,Access*);
        /* 检查名称是否和父类公有方法相同 */
        if(!Access_getIsPrivate(access)){
            assert(!Class_getMethod(self->private->super,name));
        }
        Method* selector = Method_new(NULL, self, name, function);
        HashMap_put(self->private->methods,(Object*)name,(Object*)selector);
    }
    va_end(parms);
}
/**
 * 传入name和对象类型的值,利用反射创建对象.
 * @param self
 * @param ...
 * @return
 */
void* Class_newInstanceWithFieldInfos(const Class1 *self, ...){
    Object* object = Class_newInstance(self);
    String* name;
    va_list  parms;
    Object* newValue;
    va_start(parms,self);
    while((name = va_arg(parms,String*))){
        newValue = va_arg(parms,Object*);
        Object_setValue(object, name, newValue, NULL);
    }
    va_end(parms);
    return object;
}



