//
// Created by kenhuang on 2019-12-02.
//

#ifndef OS_COMPARATOR_H
#define OS_COMPARATOR_H

#include "../Object.h"
INTERFACE(Comparator, Interface)
    int (*compare)(Object* o1, Object* o2);
    boolean (*equals)(Object* obj);
    Comparator* (*reversed)();
    Comparator* (*thenComparing)(Comparator* other);
//    Comparator* (*thenComparing1)(Function keyExtractor,Comparator keyComparator);
//    Comparator* thenComparingInt(ToIntFunction keyExtractor);
//    Comparator* thenComparingLong(ToLongFunction keyExtractor);
//    Comparator* thenComparingDouble(ToDoubleFunction keyExtractor);
END

PUBLIC_STATIC(Comparator, Interface)

END

#endif //OS_COMPARATOR_H
