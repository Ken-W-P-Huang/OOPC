//
// Created by kenhuang on 2018/6/2.
//

#ifndef  Point_H
#define  Point_H

#include "foundation/Object.h"

public_start(Point,Object)
    int x, y;				/* 坐标 */
public_end


void Point_draw (const Point * self);
Point* Point_newWithXY(void *_self, int x, int y);

#endif //Point_H