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
    void set_LTRB() override;
public:
    Circle();
    Circle(QPoint center, int radius);
    Circle(QPoint point1,QPoint point2);
    void draw(QPen& pen, QPixmap& pix) override;
    ~Circle();
    void translate(int dx, int dy) override;
    void rotate(int x, int y, int r) override;
    void scale(int x, int y, float s) override;
};

#endif // CIRCLE_H
