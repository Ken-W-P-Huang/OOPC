//#include <stdio.h>
//#include <tic.h>

#include "foundation/Foundation.h"
#include "debug.h"
#include <signal.h>
#include "type.h"
/***********************************************************************************************************************
 *  实现Runnable接口的Person类
 **********************************************************************************************************************/
PUBLIC(Person, BinaryTree)
END

PUBLIC_STATIC(Person, BinaryTree)
    IMPLEMENTS(Runnable);
#if defined(__POLYMORPHISM__)
    Person* (*init)(Person* self,char* name);
#endif
END

PRIVATE(Person)
    char*name;
END

static void deinit(Person *self) {

}

static void run(Person* self){
    printf("Hello,%s!\n",self->p->name);
    printf("%d\n",Bool_getValue(BoolClass->TRUE));
//    destroy(self);
}


Person *Person_init(Person *self,char* name) {
    if (Object_init((Object*)self)) {
        self->p->name = name;
    }
    return self;
}


STATIC_INIT(Person)
    implements((ClassType*)PersonClass,(Object*)&(PersonClass->Runnable),(ClassType*)RunnableClass,run,NULL);
END
/***********************************************************************************************************************
 *
 **********************************************************************************************************************/
//extern void asm_print(char*, int);
//void c_print(char *str)
//{
//    int len = 4;
//    asm_print(str, len);
//}
int print(int e ,int f,int a){
    printf("good %d %d %d\n",e,f,a);
    return 1000;
}
int invoke(Function f,...){
    va_list parms;
    int a ;
    int b ;
    va_start(parms,f);
    b = va_arg(parms,int);
    while (true) {
        a = va_arg(parms,int);
        if (a == 0) {
            break;
        }
//        asm("push %%rbp":"=a"(a):"a"(a));
    }
    asm("movl $50, %%edi":);
    asm("movl $20, %%esi":"=a"(a):"a"(a));
    asm("movl $100, %%edx":"=a"(a):"a"(a));
//    if (a > 0) {
//        asm("movl $50, %%edi":);
//    }
//    if (a > 0) {
//        asm("movl $20, %%esi":"=a"(a):"a"(a));
//    }
//
//    if (a > 0) {
//        asm("movl $100, %%edx":"=a"(a):"a"(a));
//    }
//    struct A {
//        int e;
//        int f;
//        int a;
//    };
//    struct A dd = {
//          100,
//          50,
//          10
//    };
    int i = f();
    va_end(parms);
    return  i;
}

int doit(int a ,int b){
    return invoke(print,a,b);
}

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

/**
 * Person实现了Runnable接口，以供Thread调用。
 */
void testThread(){
    extend((ClassType*)PersonClass,BinaryTreeClass);
    Person* person = Person_init((Person*)alloc((ClassType*)PersonClass),"Tom");
    Person* person1 = Person_init((Person*)alloc((ClassType*)PersonClass),"Jerry");
    Thread* thread = Thread_init((Thread*)alloc((ClassType*)ThreadClass),( Object*)person);
    Thread* thread1 = Thread_init((Thread*)alloc((ClassType*)ThreadClass),(Object*)person1);
    Thread_start(thread);
    Thread_start(thread1);
    for (int i = 0; i < 50; ++i) {
        printf("Main\n");
    }


}

int main() {
    initEnvironment();
    testThread();
//    int* a = NULL;
//    long b = 0;
//    int i;


//    missedSubList = {(ClassType*)ExceptionClass};
//    doit(10,20);
//    printf("%s\n", getFunctionName(ObjectClass->p));
//    printf("%s\n", getFunctionName(ObjectClass->size));
//    printf("%s\n", getFunctionName(ObjectClass->name));
//    asm ("pusha;movl $4, %eax;movl $1, %ebx;movl str, %ecx;movl $12, %edx;movl $4, %eax;int $0x80;mov %eax, count;popa ");
//    int in_a = 1, in_b = 2, out_sum;
//    /* out_sum = in_a + in_b  (扩展内联汇编寄存器的前缀是%%) */
//    char str[] = "addl %%ebx, %%eax";
//    asm(str: "=a"(out_sum): "a"(in_a), "b"(in_b));
//    printf("sum is %d\n", out_sum);
//    printf("sum is %d\n", invoke(print,10,20,100));
//    Object* object = Object_init(alloc(ObjectClass));
//    printf("isObject(0):%d\n",isObject(0));
//    printf("isObject(object):%d\n",isObject(object));
//    destroy(object);
    Person* person = Person_init((Person*)alloc((ClassType*)PersonClass),"Tom");
    printf("isKindOf(person,BinaryTreeClass):%d\n",isKindOf(person,PersonClass));
    printf("isMemberOf(person,BinaryTreeClass):%d\n",isMemberOf(person,PersonClass));
    printf("isKindOf(person,BinaryTreeClass):%d\n",isKindOf(person,BinaryTreeClass));
    printf("isMemberOf(person,BinaryTreeClass):%d\n",isMemberOf(person,BinaryTreeClass));
    printf("isKindOf(person,ObjectClass):%d\n",isKindOf(person,ObjectClass));
    printf("isMemberOf(person,ObjectClass):%d\n",isMemberOf(person,ObjectClass));
    destroy(person);
    return 0;
}




