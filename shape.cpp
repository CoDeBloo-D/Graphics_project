#include "shape.h"

Shape::Shape() {
    type = SHAPE;
}

Shape::~Shape() {
    type = -1;
}

void Shape::draw(QPen &pen, QPixmap &pix) {
    return;
}

QPoint Shape::translate_Point(int dx, int dy, QPoint point) {
    QPoint newPoint;
    newPoint.setX(point.x() + dx);
    newPoint.setY(point.y() + dy);
    return newPoint;
}

QPoint Shape::rotate_Point(int x, int y, int r, QPoint point) {
    QPoint newPoint;
    double rad = ((double)r * 3.14159265358) / 180;
    newPoint.setX((int)((double)x + (point.x() - x) * cos(rad) - (point.y() - y) * sin(rad)));
    newPoint.setY((int)((double)y + (point.x() - x) * sin(rad) - (point.y() - y) * cos(rad)));
    return newPoint;
}

QPoint Shape::scale_Point(int x, int y, float s, QPoint point) {
    QPoint newPoint;
    newPoint.setX((int)((float)x + (point.x() - x) * s));
    newPoint.setY((int)((float)y + (point.y() - y) * s));
    return newPoint;
}
