//
// Created by kenhuang on 2018/6/3.
//

#include "Environment.h"
#include "collection/ArrayList.h"
#include "exception/Exception.h"
#include "type/Bool.h"
#include "type/Byte.h"
#include "type/Character.h"
#include "type/Double.h"
#include "type/Float.h"
#include "collection/HashMap.h"
#include "type/Integer.h"
#include "type/Long.h"
#include "type/Short.h"
#include "thread/Thread.h"
#include "../Singleton.h"

/**
 *  生成各个类的class，由于使用pthread_once_t以及考虑到可能使用的单例模式，整个应用只能对环境创建销毁一次。
 * @param isReflectable 用于设置ObjectClazz中的全局变量
 */
void initEnvironment(bool isReflectable){
    Class_registerObjectClazz(isReflectable);

    Class_registerPointClazz();
    Class_registerCircleClazz();
    Class_registerPersonClazz();
    Class_registerSingletonClazz();
    printf("Environment setup is complete!\n");
}
void destroyEnvironment(){
    printf("warning:the environment will be destroyed!\n");
    Class_unregisterPersonClazz();
    Class_unregisterCircleClazz();
    Class_unregisterPointClazz();
    Class_unregisterSingletonClazz();
    Class_unregisterObjectClazz();
}