//
// Created by kenhuang on 2018/6/3.
//

#ifndef  Circle_H
#define  Circle_H

#include "Point.h"
public_start(Circle,Point)
    double radius;
public_end
Circle* Circle_newWithXY(Circle *_self, int x, int y);
#endif //Circle_H

