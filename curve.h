#ifndef CURVE_H
#define CURVE_H
#include "shape.h"

class Curve : public Shape
{
public:
    Curve();
    void translate(int dx, int dy);
    void rotate(int x, int y, int r);
    void scale(int x, int y, int s);
};

#endif // CURVE_H
