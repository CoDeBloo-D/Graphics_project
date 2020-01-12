#ifndef OVAL_H
#define OVAL_H

#include "shape.h"
#include<QPixmap>
#include<QPen>
#include<QPoint>
#include<QPainter>

class Oval : public Shape
{
private:
    QPoint center;
    int a;
    int b;
    int rotateDegrees;
    void set_Pixel(int x, int y, QPen& pen, QPixmap& pix);
    void set_LTRB() override;

public:
    Oval();
    Oval(QPoint point1, QPoint point2);
    Oval(QPoint center,int a, int b);
    ~Oval();
    void draw(QPen& pen, QPixmap& pix) override;
    void translate(int dx, int dy) override;
    void rotate(int x, int y, int r) override;
    void scale(int x, int y, float s) override;
};

#endif // OVAL_H
