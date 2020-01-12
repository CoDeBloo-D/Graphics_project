#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include "line.h"


class Rectangle : public Shape
{
private:
    QPoint point1;
    QPoint point2;
    QPoint point3;
    QPoint point4;
    Line line1,line2,line3,line4;
    int drawMethod;
    void set_LTRB() override;

public:
    Rectangle();
    Rectangle(QPoint point1,QPoint point2);
    ~Rectangle();
    void setDrawMethod(int m);
    void draw(QPen& pen, QPixmap& pix) override;
    void translate(int dx, int dy) override;
    void rotate(int x, int y, int r) override;
    void scale(int x, int y, float s) override;
};

#endif // RECTANGLE_H
