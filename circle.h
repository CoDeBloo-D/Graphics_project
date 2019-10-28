#ifndef CIRCLE_H
#define CIRCLE_H

#include"shape.h"
#include<QPixmap>
#include<QPen>
#include<QPoint>
#include<QPainter>

class Circle : public Shape
{
private:
    QPoint center;
    int radius;
    void set_Pixel(int x, int y, QPen &pen, QPixmap &Pix);
public:
    Circle();
    Circle(QPoint center, int radius);
    Circle(QPoint point1,QPoint point2);
    void draw(QPen& pen, QPixmap& pix) override;
    ~Circle();
    void translate(int dx, int dy);
    void rotate(int x, int y, int r);
    void scale(int x, int y, float s);
};

#endif // CIRCLE_H
