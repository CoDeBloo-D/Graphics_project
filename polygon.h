#ifndef POLYGON_H
#define POLYGON_H

#include"shape.h"
#include"line.h"
#include<QPixmap>
#include<QPen>
#include<QPoint>
#include<QPainter>
#include<QVector>

class Polygon : public Shape
{
private:
    QVector<QPoint> points;
    int drawMethod;
public:
    Polygon();
    Polygon(QVector<QPoint> v);
    ~Polygon();
    void setDrawMethod(int m);
    void draw(QPen& pen, QPixmap& pix) override;
    void translate(int dx, int dy);
    void rotate(int x, int y, int r);
    void scale(int x, int y, float s);
};

#endif // POLYGON_H
