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
    QPoint transformCenter;
    QPoint LTPoint, RBPoint;
    QPoint translate_Point(int dx, int dy, QPoint point);
    QPoint rotate_Point(int x, int y, int r, QPoint point);
    QPoint scale_Point(int x, int y, float s, QPoint point);
    virtual void set_LTRB() = 0;

public:
    Shape();
    ~Shape();
    int getType();
    QPoint getCenter();
    QPoint getLT();
    QPoint getRB();
    virtual void draw(QPen& pen, QPixmap& pix) = 0;
    virtual void translate(int dx, int dy) = 0;
    virtual void rotate(int x, int y, int r) = 0;
    virtual void scale(int x, int y, float s) = 0;
    virtual void clip(QPoint point1, QPoint point2, std::string algorithm);
    bool in_DraggingArea(QPoint cursor);
};

#endif // SHAPE_H
