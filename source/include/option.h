//
// Created by kenhuang on 19-10-21.
//

#ifndef OOPC_OPTION_H
#define OOPC_OPTION_H
/* 启用final和类类型功能 */
#define __CLASS_ATTRIBUTE__
/* 启用hashCode equal，update等基本方法进行重写，这些方法决定能否使用HashMap以及观察者模式。开启多态会覆盖此开关 */
#define __OVERRIDE__
/* 启用反射功能，依赖于OVERRIDE */
#define __REFLECT__
/* 启用多态，无依赖 */
#define __POLYMORPHISM__
/* 启用调试打印功能，无依赖 */
#define __DEBUG__
#endif //OOPC_OPTION_H
