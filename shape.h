#ifndef SHAPE_H
#define SHAPE_H

#include <QPixmap>
#include <QPen>
#include <QPoint>
#include <QVector>

class Shape
{
protected:
    enum {SHAPE, LINE, CIRCLE, RECTANGLE, OVAL, POLYGON, CURVE};
    int type;
    QPoint translate_Point(int dx, int dy, QPoint point);
    QPoint rotate_Point(int x, int y, int r, QPoint point);
    QPoint scale_Point(int x, int y, float s, QPoint point);
public:
    Shape();
    ~Shape();
    virtual void draw(QPen& pen, QPixmap& pix);
};

#endif // SHAPE_H
