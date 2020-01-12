#ifndef CURVE_H
#define CURVE_H

#include "shape.h"
#include<QPixmap>
#include<QPen>
#include<QPoint>
#include<QPainter>

class Curve : public Shape
{
private:
    QVector<QPoint> points;
    int drawMethod;
    void set_Pixel(double x, double y, QPen& pen, QPixmap& pix);
    void set_LTRB() override;

public:
    Curve();
    Curve(QVector<QPoint> v);
    ~Curve();
    void setDrawMethod(int m);
    void draw(QPen& pen, QPixmap& pix) override;
    void translate(int dx, int dy) override;
    void rotate(int x, int y, int r) override;
    void scale(int x, int y, float s) override;
};

#endif // CURVE_H
