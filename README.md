 &nbsp;&nbsp;&nbsp;&nbsp;一个面向对象的C语言框架。越高级的面向对象的特性的实现所需要的代码越多。如果只需要封装和继承这两种特性，可以将source/include/option.h所有的特性全部注释。
 面向对象的静态含义和c语言的静态含义不同。 面向对象的静态可以简单理解为和类对象相关，而c语言的静态含义类似于面向对象的私有。
# 三大特性的实现
1. 封装

    .h文件
    ``` c  
    typedef struct {
        //成员p所指向的结构体存放的是私有属性。
        struct BinaryTree_P *p;
    } BinaryTree;
    ```
    .c文件
    ``` c 
    typedef struct BinaryTree_P {
        BinaryTreeNode* root;
    } BinaryTree_P;
    ```
2. 继承

    .h文件
    ``` c 
    typedef struct {
        Object _;
        struct BinaryTree_P *p;
    } BinaryTree;
    
    typedef struct {
        ObjectClassType _;
    } BinaryTreeClassType;
    ```
    &nbsp;&nbsp;&nbsp;&nbsp;BinaryTree继承自Object。BinaryTree和BinaryTree_P结构体分别存放对象公有属性和私有属性，其实例即为对象；
    BinaryTreeClassType存放对象方法，类属性和类方法等信息，其实例为类对象。
    可根据实际需要，按照对象私有属性的定义方式在.c文件中封装类对象的私有属性。

3. 多态
    ``` c 
    typedef struct {
        ObjectClassType _;
        BinaryTree * (*init)(BinaryTree *self,int (*compare)(Object*, Object*));
        void  (*insert)(BinaryTree *self, Object* data);
        void  (*clear)(BinaryTree *self);
    } BinaryTreeClassType;
    ```
    &nbsp;&nbsp;&nbsp;&nbsp;需要在第4点所述的clinit函数中使用赋值语句或者调用Class_addFunctions函数，将方法赋值给BinaryTreeClass函数指针成员。
    假设RedBlackTree继承了BinaryTree，则RedBlackTreeClassType结构体中必定有BinaryTreeClassType _;的定义。
    此时使用(BinaryTreeClassType*)getClass()->insert()进行调用。
    如果不需要多态功能，可以不必在此结构体中定义函数指针成员以节省内存，直接使用.h文件中的函数声明进行调用即可。
    
    示例：source/foundation/type/String.c对hashCode和equals方法的重写。

4. 类对象

    &nbsp;&nbsp;&nbsp;&nbsp;对象只存放数据，不存放方法，否则会浪费很多内存。类对象记录了创建对象所需要的基本信息，对象方法，静态属性，静态方法以及类初始化函数（
    类似于Java的静态代码块）。内容如下：
    ``` c 
    /* 类初始化函数 */
    static void clinit();
    static BinaryTreeClassType BinaryTreeClazz[] = {
    {
                    NULL,
                    "BinaryTree",
                    sizeof(BinaryTree),
                    sizeof(_BinaryTree),
                    offsetof(BinaryTree, p),
                    sizeof(BinaryTreeClassType),
    #if defined(__CLASS_ATTRIBUTE__)
                    REAL,
                    false,
    #endif
                    clinit,
                    (void (*)(Object *)) deinit,
    }
    };
    BinaryTreeClassType *const BinaryTreeClass = BinaryTreeClazz;
    static void clinit() {
    
    }
    ```
    &nbsp;&nbsp;&nbsp;&nbsp;BinaryTreeClass即为类对象。为了和普通对象区分开，而按类的命名规则命名。可以使用该类对象来创建对象。使用前还需要在主函数中调用extend函数
    以明确该类的父类为ObjectClass，详见source/foundation/Foundation.c。extend的函数会自动调用clinit函数，从而实现静态代码块的执行。
    读者可以在clinit函数中添加面向对象概念下的静态代码，例如对类属性进行赋值。
    
    以上创建一个完整类的方法看似复杂，其实可以利用IDE的模版文件或者C语言的宏定义进行简化。简化后的代码量和面向对象语言差别不大，请参见其他第1点使用宏进行简化。

5. 创建/删除对象
    ``` c 
    BinaryTree* tree = BinaryTree_init((BinaryTree*)alloc(BinaryTreeClass));
    ```
    &nbsp;&nbsp;&nbsp;&nbsp;创建对象的方式和OC类似，删除对象的方式和C++类似。alloc函数为新对象分配空间。这片空间包含新对象自行定义的以及从父类继承过来的
    所有公有属性和私有属性。而BinaryTree_init则对新创建的对象进行初始化。C语言的函数不能同名，读者可根据需要自行添加参数不同
    的初始化函数。建议初始化函数命名规则为类名_initXXX。
    
    &nbsp;&nbsp;&nbsp;&nbsp;从类对象的成员可知，每个类必须强制定义deinit函数。该函数为析构函数，用于释放本对象的属性所占用的空间等。
    删除对象时，只需调用destroy()函数。该函数会循环调用该对象及其父类的deinit函数，释放所有的空间。
    也可以调用retain和release函数，采用引用计数的方式管理内存。

# 其他特性
1. 类的类型和final

    &nbsp;&nbsp;&nbsp;&nbsp;本框架将类分为实体类（可以实例化的类），抽象类和接口，其定义为ClassType的type成员。alloc函数无法为类型为抽象类的类对象创建对象，但可以为
    接口创建对象，此时创建的对象为Java所谓的匿名接口。isFinal为true的类对象不允许有子类。将其作为super参数调用extend函数时，该函数会报错。

2. 接口

    &nbsp;&nbsp;&nbsp;&nbsp;接口是一种特殊的类，其对象实例成员只有公共方法。接口一般作为成员寄生在其他可实例化的类对象上，通过类对象的实例化来实例化接口本身。
    但也有特殊情况，即接口可以直接被实例化，此时表示使用匿名接口。
    参考source/main.c：Person类实现了Runable接口。

3. 反射

    已实现属性反射，方法反射尚未实现。

4. 注解与切面编程。

    需要安装编译器扩展，未深入研究。
    https://aspectc.org/Download.php

# 其他
1. 使用宏进行简化

    详见source/foundation/util/type.h的宏定义和source/main.c对Person类的简化结果。

2. 功能开关：

    &nbsp;&nbsp;&nbsp;&nbsp;用户可根据需要修改source/foundation/util/option.h文件的相应开关，开启/关闭final、抽象、多态，反射、打印等功能。

3. 缺点：

    - 编译时会出现太多类型不匹配的警告信息。建议在确认可以强制转换后，使用强制转换消除。
    - 必须按照接口、方法，类属性的顺序定义类对象。
    - 使用宏定义类时STATIC_INIT/PUBLIC/PUBLIC_STATIC必须成套出现，并且一套.h/.c文件对只能定义一个类。
googletest的cmake使用VERSION_GREATER_EQUAL，要求的版本为3.7。建议CLion版本为2018及以上，否则无法正常测试。





