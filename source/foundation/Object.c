//
// Created by kenhuang on 2018/6/22.
//

#include "Object.h"

PRIVATE(Object)
    /* 指向对象的类 */
    const ClassType *class;
    /* 引用计数 */
    unsigned  int refCount;
#if defined(__OVERRIDE__) || defined(__POLYMORPHISM__)
    /* 观察者链表 */
    LinkedList *observerList;
#endif
END

PRIVATE(ClassType)
    size_t totalSize;
    const ClassType * super;
    int interfaceAmount;
#ifdef __REFLECT__
    /* 子类表,forName */
    HashMap* subClasses;
    HashMap* fields;
    HashMap* methods;
    /*Key为interface，value为接口在类中的偏移，方便定位*/
    HashMap* interfaces;
#endif
END
static Lock* lock;
static Exception* exception;
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void handleSigsegv(int param){
    signal(SIGINT, SIG_DFL);
    throw(exception);
}

static void deinit(Object* self){
#if defined(__OVERRIDE__) || defined(__POLYMORPHISM__)
    if(self->p->observerList){
        /* 用户必须自行清空观察者 */
        assert(LinkedList_getSize(self->p->observerList) == 0);
        destroy((Object *)self->p->observerList);
    }
#endif
}

#if (defined(__OVERRIDE__) || defined(__POLYMORPHISM__)) && defined(__REFLECT__)
static char * getFunctionName(Function method){
    void *buffer[1] = {method};
    char* methodName = NULL;
    char **symbols = backtrace_symbols(buffer, 1);
    if (symbols == NULL) {
        perror("backtrace_symbols");
    } else {
        methodName = symbols[0];
        free(symbols);
    }
    return methodName;
}
static size_t getDataSize(const ClassType *self){
    size_t size;
    if(self == (ClassType*)BoolClass){
        size = sizeof(bool);
    }else if(self == (ClassType*)IntegerClass){
        size = sizeof(int);
    }else if(self == (ClassType*)ByteClass){
        size = sizeof(byte);
    }else if(self == (ClassType*)CharacterClass){
        size = sizeof(char);
    }else if(self == (ClassType*)DoubleClass){
        size = sizeof(double);
    }else if (self == (ClassType*)FloatClass){
        size = sizeof(float);
    }else if(self == (ClassType*)LongClass){
        size = sizeof(long);
    }else if(self == (ClassType*)PointerClass){
        size = sizeof(void*);
    }else if(self == (ClassType*)ShortClass){
        size = sizeof(short);
    } else if (self == (ClassType*)SizeTypeClass) {
        size = sizeof(size_t);
    } else{
        size =  self->size;
    }
    return size;
}
#endif
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
#if defined(__OVERRIDE__) || defined(__POLYMORPHISM__)
char* Object_toString(Object *self){
    return (char*) getClass(self)->name;
}

bool Object_equals(Object *self, Object *obj){
    return self == obj;
}
int Object_hashCode(Object *self){
    return (int)self;
}
void Object_update(Object* self,Object* subject,int dataEnum){
        printf("update from Object %d\n",dataEnum);
}
// 观察者模式
/**
 *  将observer加入subject中的observer链表
 * @param self
 * @param observer
 * @return
 */
void Object_addObserver(Object *self, Object *observer) {
    assert(!isKindOf(self,Class));
    if(!self->p->observerList){
        self->p->observerList = LinkedList_init(alloc((ClassType*)LinkedListClass));
    }
    LinkedList_add(self->p->observerList,observer);
}

/**
 * 将observer踢出subject中的observer链表
 * @param self
 * @param observer
 * @return
 */
void Object_removeObserver(Object *self, Object *observer) {
    LinkedList_remove(self->p->observerList,observer);
}

/**
 * 通知观察者
 * @param pObject
 */
void Object_notifyObservers(Object *self, int dataEnum){
    Object* observer;
    const ClassType* class;
    LinkedList* list = self->p->observerList;
    unsigned  size = LinkedList_getSize(list);
    for (unsigned int i = 0; i < size; ++i) {
        observer = LinkedList_get(list,i);
        class = getClass(observer);
        (class->update)(observer,self,dataEnum);
    }
}
#endif

Object *Object_init(Object *self) {
    if (self) {
        self->p->refCount++;
    }
    //不要在这里初始化LinkedList，会死循环。
    return self;
}

Object* Object_clone(Object *self){
    const ClassType* class = getClass(self);
    void* obj = malloc(class->size);
    memcpy(obj,self,class->size);
    return obj;
}

const ClassType * getClass(const Object *self){
    return self->p->class;
}

bool isKindOf(const Object *self, const ClassType *clazz){
    if (clazz == ObjectClass) {
        return  true;
    }

    const ClassType * objectClass = self->p->class;
        while(objectClass != ObjectClass){
            if (objectClass == clazz) {
                return true;
            }
            objectClass = objectClass->p->super;
        }

    return false;
}

bool isMemberOf(const Object *self, const ClassType *clazz){
    return self->p->class == clazz;
}

void* retain(Object *self){
    if(self){
        self->p->refCount++;
        _DEBUG("an instance of %s is retained,refCount=%u", getClass(self)->name,self->p->refCount);
    }
    return self;
}
void* release(Object *self){
    if(self && 0 == --(self->p->refCount)){
        _DEBUG("an instance of %s is released,refCount=%u", getClass(self)->name,self->p->refCount - 1);
        destroy(self);
        _DEBUG("an instance of %s is freed", getClass(self)->name);
        return NULL;
    }
    return self;
}


void * cast(Object *self, const ClassType *clazz){
    if (isObject(self)) {
        assert(isKindOf(self, clazz));
        return self;
    }
    return NULL;
}
#if (defined(__OVERRIDE__) || defined(__POLYMORPHISM__)) && defined(__REFLECT__)
bool respondsTo(const Object *self, const String *name){
    const  ClassType * class = getClass(self);
    return Class_getMethod(class,name) != NULL;
}
#endif
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
/**
* 往Class中填充对象属性类型。
* @param self
* @param ... 次序为属性名，属性类型，属性访问权限Access...。属性类型必须为Class实例，例如int对应的类型为IntegerClass，指针对应的类型
* 为Pointer。必须按公有属性到私有属性依次填充。
*/
void Class_fillDeclaredFields(const ClassType *self, ...){
#if (defined(__OVERRIDE__) || defined(__POLYMORPHISM__)) && defined(__REFLECT__)
    va_list  parms;
    char* nameArray;
    String* name;
    ClassType* type;
    size_t offset ;
    size_t length = 0;
    size_t remainder = 0;
    Access* access;
    bool privateEncountered = false;
    /* 从父类"_"之后开始计算 */
    offset =self->p->super->size;
    va_start(parms,self);
    while ((nameArray = va_arg(parms,char*)) && nameArray != NULL) {
        /* 检查属性是否已经定义 */
        name = String_init(alloc((ClassType*)StringClass),nameArray);
        assert(!Class_getDeclaredField(self,name));
        type = va_arg(parms,ClassType*);
        access = va_arg(parms,Access*);
        if(!Access_getIsPrivate(access)){
            assert(!Class_getField(self->p->super,name));
        }
        if(!privateEncountered && Access_getIsPrivate(access)){
            privateEncountered = true;
            offset = 0;
        }
        length = getDataSize(type);
        remainder = offset % length;
        if (remainder != 0){
            offset += remainder;
        }
        Field * field = Field_init(alloc((ClassType*)FieldClass), self, name, type, offset, length, access);
        HashMap_put(self->p->fields,(Object*)name,(Object*)field);
        offset += length;
    }
    va_end(parms);
#endif
}
/**
 * 往Class中填充对象方法。
 * @param self
 * @param ...  次序为方法指针，方法访问权限Access。
 */
void Class_fillDeclaredMethods(const ClassType *self, ...){
#if (defined(__OVERRIDE__) || defined(__POLYMORPHISM__)) && defined(__REFLECT__)
    va_list  parms;
    String* name;
    Function function;
    Access* access;
    va_start(parms,self);
    while ((function = va_arg(parms,Function))) {
        name = String_init(alloc((ClassType*) StringClass),getFunctionName(function));
        /* 检查方法名是否已经定义，防止重复插入 */
        assert(!Class_getDeclaredMethod(self,name));
        access = va_arg(parms,Access*);
        /* 检查名称是否和父类公有方法相同 */
        if(!Access_getIsPrivate(access)){
            assert(!Class_getMethod(self->p->super,name));
        }
        Method* selector = Method_init(alloc((ClassType*)MethodClass), self, name, function);
        HashMap_put(self->p->methods,(Object*)name,(Object*)selector);
    }
    va_end(parms);
#endif
}
#if (defined(__OVERRIDE__) || defined(__POLYMORPHISM__)) && defined(__REFLECT__)
/**
 * 通用get方法，Access位域属性的name为"access"
 * @param self
 * @param name
 * @return
 */
void *getValue(Object *self, String *name, Exception *exception) {
    const ClassType* class = getClass(self) ;
    const ClassType* super = class;
    void** valuePointer = NULL ;
    Field* field = Class_getDeclaredField(super,name);
    while(!field  && super != ObjectClass){
        super = Class_getSuperClass(super);
        field = Class_getDeclaredField(super,name);
    }
    if(field){
        Access* access = Field_getAccess(field);
        if(Access_getReadable(access)){
            const ClassType* type = Field_getType(field);
            size_t offset = Field_getOffset(field);
            if(Access_getIsPrivate(access)){
                valuePointer = (void **)((byte*)(*(void**)((byte *)self + class->privateOffset)) + offset);
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
void setValue(Object *self, String *name, void *newValue, Exception *exception) {
    const ClassType* class = getClass(self) ;
    const ClassType* super = class;
    Field* field = Class_getDeclaredField(super,name);
    while(field == NULL  && super != ObjectClass){
        super = Class_getSuperClass(super);
        field = Class_getDeclaredField(super,name);
    }
    if(field != NULL){
        Access* access = Field_getAccess(field);
        if(Access_getWritable(access)){
            const ClassType* type = Field_getType(field);
            void** destination;
            size_t offset = Field_getOffset(field);
            size_t length = Field_getLength(field);
            if(Access_getIsPrivate(access)){
                destination = (void **)((byte*)(*(void**)((byte *)self +  class->privateOffset)) + offset);
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
* Class_forName调用,遍历整个Class继承树，查找匹配的Class
* @param className
* @return
*/
static const ClassType* searchClassInClassTree(String* className){
    ClassType* class,*super;
    LinkedList* stack = LinkedList_init(alloc((ClassType*)LinkedListClass));
    LinkedList_push(stack,(Object*)ObjectClass);
    ClassType_P* p;
    while(LinkedList_getSize(stack) > 0){
        super = (ClassType*)LinkedList_pop(stack);
        p = super->p;
        class = (ClassType*)HashMap_get(p->subClasses,(Object*)className);
        if(class){
            return class;
        }else{
            if(HashMap_getSize(p->subClasses) > 0){
                struct HashMapEntryIterator* iterator = HashMap_getIterator(p->subClasses);
                while(HashMapEntryIterator_hasNext(iterator)){
                    LinkedList_push(stack,HashMapEntry_getValue(HashMapEntryIterator_next(iterator)));
                }
                destroy((Object*)iterator);
            }
        }
    }
    destroy((Object*)stack);
    return NULL;
}
/**
 * Class_forName调用
 * @param className
 * @return
 */
//todo 注入机器码/动态链接库？
static const ClassType* loadClass(String* className){
    ClassType* class = NULL;
    return class;
}

const ClassType* Class_forName(String *className){
    const ClassType* class = searchClassInClassTree(className);
    if(!class){
        class = loadClass(className);
    }
    return  class;
}

static Field * getField(HashMap *map, String *name,bool publicOnly){
    Field* result = (Field*)HashMap_get(map,(Object*)name);
    if (result != NULL && publicOnly) {
        Access* access = Field_getAccess(result);
        if (Access_getIsPrivate(access) != false) {
            return NULL;
        }
    }
    return result;
}
/**
 * 获取当前类定义的指定属性不包含父类属性
 * @param self
 * @param name
 * @return
 */
Field * Class_getDeclaredField(const ClassType *self, String *name) {
    Field* field = NULL;
    field = getField(self->p->fields,name,false);
    return  field;
}
/**
 * 从当前类及其父类获取指定公共属性, 不包含私有属性
 * @param self
 * @param name
 * @return
 */
Field* Class_getField(const ClassType *self, String *name) {
    Field* field;
    const ClassType* super = self;
    field = getField(super->p->fields, name,true);
    while(super != ObjectClass && !field){
        super = super->p->super;
        field = getField(super->p->fields, name,true);

    }
    return field;
}

/**
 * 从指定链表中获取指定名称的Method
 * @param methodMap
 * @param name
 * @param parameterTypes
 * @return
 */
static Method* getMethod(HashMap *methodMap, const String *name, ClassType **parameterTypes){
    return (Method*)HashMap_get(methodMap,(Object*)name);
}
/**
 * 从本类所有方法中获取name指定的方法
 * @param self
 * @param name
 * @param isPrivate
 * @return
 */
Method * Class_getDeclaredMethod(const ClassType *self, String *name) {
    Method *selector = NULL;
    selector = getMethod(self->p->methods, name,NULL);
    return selector;
}

/**
 * 从当前类及其父类的公共方法中获取指定公共方法。
 * @param self
 * @param name
 * @param isPrivate
 * @return
 */
Method* Class_getMethod(const ClassType *self, const String *name) {
    Method *selector;
    const ClassType* super = self;
    selector = getMethod(super->p->methods, name,NULL);
    if (super != ObjectClass && (!selector || Access_getIsPrivate(Method_getAccess(selector)))) {
        do {
            super = super->p->super;
            selector = getMethod(super->p->methods, name,NULL);
        } while (super != ObjectClass);
    }
    return selector;
}

/**
 * 传入name和对象类型的值,利用反射创建对象.
 * @param self
 * @param ...
 * @return
 */
void* Class_newInstanceWithFields(const ClassType *self, ...){
    Object* object = alloc(self);
    String* name;
    va_list  parms;
    Object* newValue;
    va_start(parms,self);
    while((name = va_arg(parms,String*))){
        newValue = va_arg(parms,Object*);
        setValue(object, name, newValue, NULL);
    }
    va_end(parms);
    return object;
}
#endif

static void classClinit(){
#if (defined(__OVERRIDE__) || defined(__POLYMORPHISM__)) && defined(__REFLECT__)
    Class_fillDeclaredFields(ObjectClass,
                             "class",PointerClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "refCount",IntegerClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "observerList",PointerClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods(ObjectClass,Object_init,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Object_toString,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Object_equals,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Object_hashCode,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Object_update,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Object_clone,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Object_addObserver,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Object_removeObserver,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Object_notifyObservers,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              respondsTo,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              setValue,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              getValue,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
    Class_fillDeclaredFields(Class,"name",PointerClass,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                             "size",SizeTypeClass,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                             "privateSize",SizeTypeClass,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                             "privateOffset",IntegerClass,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                             "classSize",SizeTypeClass,AccessClass->READABLE_UNWRITABLE_PUBLIC,
#if defined(__CLASS_ATTRIBUTE__)
                             "type",IntegerClass,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                             "isFinal",ShortClass,AccessClass->READABLE_UNWRITABLE_PUBLIC,
#endif
                             NULL);
    Class_fillDeclaredMethods(Class,
                              Class_getSuperClass,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Class_fillDeclaredFields,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Class_fillDeclaredMethods,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Class_forName,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Class_getDeclaredField,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Class_getField,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Class_getDeclaredMethod,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Class_getMethod,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Class_newInstanceWithFields,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
#endif
    /* 用于isObject方法。由于ObjectClass必须第一个初始化，此时Exception以及Lock还未初始化，不能创建对象，故将初始化放于此处*/
    exception = Exception_init(alloc((ClassType*)ExceptionClass));
    exception->type = SEGMENT_EXCEPTION;
    exception->message = "segment exception";
    lock = Lock_init(alloc((ClassType*)LockClass));
}

static void objectClinit(){

}

const ClassType * Class_getSuperClass(const ClassType *self){
    assert(self && self->p->super);
    return self->p->super;
}

static struct ClassType clazz [] = {
        {       NULL,
                .name = "Object",
                .size = sizeof(Object),
                .privateSize = sizeof(Object_P),
                .privateOffset = offsetof(Object,p),
                .classSize = sizeof(ClassType),
#if defined(__CLASS_ATTRIBUTE__)
                .type = REAL,
                .isFinal = false,
#endif
                .clinit = objectClinit,
                .deinit = deinit,
#if defined(__OVERRIDE__) || defined(__POLYMORPHISM__)
                .toString = Object_toString,
                .hashCode = Object_hashCode,
                .equals = Object_equals,
                .update = Object_update,
                .clone = Object_clone,
                .addObserver = Object_addObserver,
                .removeObserver = Object_removeObserver,
                .notifyObservers = Object_notifyObservers
#endif
        },
        {       NULL,
                .name ="Class",
                .size = sizeof(ClassType),
                .privateSize = sizeof(ClassType_P),
                .privateOffset = offsetof(ClassType,p),
                .classSize = sizeof(ClassType),
#if defined(__CLASS_ATTRIBUTE__)
                .type = REAL,
                .isFinal = false,
#endif
                .clinit = classClinit,
        }
};


ClassType * const ObjectClass = clazz;
ClassType * const Class = clazz + 1;
ClassType * const InterfaceClass = clazz;



static void initClass(ClassType*sub,ClassType*super){
#if (defined(__OVERRIDE__) || defined(__POLYMORPHISM__)) && defined(__REFLECT__)
    sub->p->subClasses = HashMap_init(alloc((ClassType*)HashMapClass));
    sub->p->methods = HashMap_init(alloc((ClassType*)HashMapClass));
    sub->p->fields = HashMap_init(alloc((ClassType*)HashMapClass));
    String* name = String_init(alloc((ClassType*)StringClass),(char*)sub->name);
    HashMap_put(super->p->subClasses,(Object*)name,(Object*)sub);
#endif
    /*载入method和field,并初始化静态变量*/
    if(sub->clinit != NULL){
        sub->clinit();
        sub->clinit = NULL;
    }
}


ClassType* extend(ClassType* sub,ClassType* super){
#if defined(__CLASS_ATTRIBUTE__)
    if (super->isFinal == true) {
        _DEBUG("%s is final and could not be extend.\n",super->name);
        raise(SIGUSR1);
    }
#endif

    /* 复制父类对象的信息到子类对象,忽略clinit及之前的信息 */
    if (sub != ObjectClass) {
        size_t offset = 0;
#if defined(__OVERRIDE__) || defined(__POLYMORPHISM__)
        if (sub == (ClassType*)StringClass) {
            //避开hashCode和equals
            offset = sizeof(void*) * 3;
        }
#endif
        memcpy((byte*)sub + Class->privateOffset + offset,(byte*)super + Class->privateOffset + offset,
               super->classSize - Class->privateOffset - offset) ;
    }
    ClassType_P * classPrivate = calloc(1,sizeof(ClassType_P));
    classPrivate->super = super;
    sub->p = classPrivate;
    /*计算对象总大小*/
    if (sub == ObjectClass) {
        classPrivate->totalSize = sub->size + sub->privateSize;
    }else {
        classPrivate->totalSize = super->p->totalSize - super->size + sub->size + sub->privateSize;
    }
     ClassType* missedSubList[] = {ObjectClass,(ClassType*)AccessClass,
#if (defined(__OVERRIDE__) || defined(__POLYMORPHISM__)) && defined(__REFLECT__)
                                   (ClassType*)MethodClass,
                                   (ClassType*)FieldClass,
                                   (ClassType*)HashMapEntryClass,(ClassType*)HashMapClass,
#endif
                                   (ClassType*)ExceptionClass,
                                   (ClassType*)PointerClass,(ClassType*)BoolClass,(ClassType*)ByteClass,
                                   (ClassType*)CharacterClass,(ClassType*)DoubleClass,(ClassType*)SizeTypeClass,
                                   (ClassType*)FloatClass,(ClassType*)IntegerClass,
                                   (ClassType*)LongClass,
                                   (ClassType*)StringClass,(ClassType*)ShortClass,Class,
                                   (ClassType*)LinkedListClass,(ClassType*) LinkedListIteratorClass,
                                   (ClassType*)LockClass};
    static const int COUNT = sizeof(missedSubList) / sizeof(ClassType*) - 1;
    static int count = 0;
    if(count < COUNT){
        count++;
    }else if (count == COUNT){
        for (int i = 0; i <= count; ++i) {
            initClass(missedSubList[i],super);
        }
        count++;
    }else{
        initClass(sub,super);
    }
    /*所有类对象的类均为Clazz*/
    static const Object_P classObject[1] ={
            {
                    .class = Class
            }
    };
    sub->_.p = (Object_P *)classObject;
    return sub;
}


void * alloc(const ClassType * clazz){
    Object * object;
    void** privatePointer;
#if defined(__CLASS_ATTRIBUTE__)
    //||clazz->type == INTERFACE 接口可以被实例化，表示使用匿名接口，并且还可以有初始化方法，表示可以有默认实现。而抽象类不行。
    if (clazz->type == ABSTRACT ) {
        _DEBUG("The type of class %s is not real and could not be allocated.\n",clazz->name);
        raise(SIGUSR1);
    }
#endif
    object = calloc(1, clazz->p->totalSize);
    const ClassType* super = clazz;
    byte* address = (byte*)object + super->size;
    if(super ->privateSize > 0){
        privatePointer = (void**)((byte*)object + super->privateOffset);
        *privatePointer = address;
    }
    while(super != ObjectClass) {
        address += super->privateSize;
        super = super->p->super;
        if(super->privateSize > 0){
            privatePointer = (void**)((byte*)object + super->privateOffset);
            *privatePointer = address;
        }
    }
    /* 对象的类对象是clazz */
    object->p->class = clazz;
    return object;
}


bool isObject(void* pointer){
    Lock_lock(lock);
    Object * o = (Object *)pointer;
    signal(SIGSEGV, handleSigsegv);
    bool isObject = false;
    try(exception){
        if (o->p->class->_.p->class == Class) {
            isObject = true;
        }
    } catch {
        _DEBUG("catch a signal");
    };
    signal(SIGSEGV,NULL);
    Lock_unlock(lock);
    return isObject;
}

void destroy(Object *self){
    if (self) {
        const ClassType* super = getClass(self);
        if (super->deinit) {
            super->deinit(self);
        }
        while(super != ObjectClass) {
            super = super->p->super;
            if (super->deinit) {
                super->deinit(self);
            }
        }
        free(self);
    }
}

void multiDestroy(Object *self,...){
    destroy(self);
    va_list list;
    va_start(list,self);
    while (true) {
        self = va_arg(list,Object*);
        if (self == NULL) {
            break;
        }
        destroy(self);
    }
    va_end(list);
}

static const size_t funPointerSize = sizeof(Function);
/**
 *
 * @param self
 * @param interfaceAddressInSelf 类对象中的接口地址,这里需要将接口当作对象进行处理
 * @param interfaceClass 要实现的接口
 * @param ... NULL作为结束符
 * @return
 */
void implements(ClassType* self, Object*interfaceAddressInSelf,ClassType* interfaceClass,...){
    va_list parameters;
    Function function = NULL;
    self->p->interfaceAmount++;
    /* 类对象上的接口实例尚未指明其接口类对象 */
    interfaceAddressInSelf->p = malloc(sizeof(Object_P));
    interfaceAddressInSelf->p->class = interfaceClass;
    Function * funPointer = (Function *)((byte*)interfaceAddressInSelf + sizeof(void*));
    va_start(parameters,interfaceClass);
    while (true) {
        function = va_arg(parameters,Function);
        if (function == NULL) {
            break;
        }
        *funPointer  = function;
        funPointer = (Function *)(((byte*)funPointer) + funPointerSize);
    }
    va_end(parameters);
}

static void* retriveInterfaceInClass(const ClassType* self,ClassType* interface){
    Object* object = NULL;
    if (self->p->interfaceAmount > 0) {
        const ClassType* clazz = Class_getSuperClass(self);
        for (int i = 0; i < self->p->interfaceAmount; ++i) {
            object = (Object*)((byte*)self + clazz->classSize);
            assert(object != NULL);
            clazz = getClass(object);
            assert(clazz != NULL);
            if (clazz == interface) {
                break;
            }
        }
    }
    return object;
}

void* getInterface(Object*self,ClassType* interface){
    const ClassType* clazz = getClass(self);
    void* obj = retriveInterfaceInClass(clazz,interface);
    /* 逐层查找接口实例 */
    if (obj == NULL) {
        while (clazz != ObjectClass) {
            clazz = (ClassType*) Class_getSuperClass(clazz);
            obj = retriveInterfaceInClass(clazz,interface);
            if (obj != NULL) {
                break;
            }
        }
    }
    return  obj;
}

/**
 * 使用此函数时应当将类属性按接口,方法，静态属性的顺序依次摆放。
 * @param self
 * @param ...
 */
void Class_addFunctions(ClassType *self, ...){
#if defined(__POLYMORPHISM__)
    va_list parameters;
    Function function = NULL;
    /*找到方法的起始位置*/
    Interface* anInterface = (Interface*)((byte*)self + Class_getSuperClass(self)->classSize);
    for (int i = 0; i < clazz->p->interfaceAmount; ++i) {
        anInterface = (Interface*)((byte*)anInterface + getClass(anInterface)->size);
    }
    Function * funPointer = (Function *)anInterface;
    va_start(parameters,self);
    while (true) {
        function = va_arg(parameters,Function);
        if (function == NULL) {
            break;
        }
        *funPointer  = function;
        funPointer = (Function *)(((byte*)funPointer) + funPointerSize);
    }
    va_end(parameters);
#endif
}