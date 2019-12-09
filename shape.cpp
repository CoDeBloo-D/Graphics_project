#include "shape.h"
#include <QDebug>

Shape::Shape() {
    type = SHAPE;
}

Shape::~Shape() {
    type = -1;
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
    //qDebug() << rad << cos(rad) << sin(rad) << endl;
    newPoint.setX((int)(0.5 + (double)x + (point.x() - x) * cos(rad) - (point.y() - y) * sin(rad)));
    newPoint.setY((int)(0.5 + (double)y + (point.x() - x) * sin(rad) + (point.y() - y) * cos(rad)));
    //qDebug() << point.x() << point.y() << endl;
    //qDebug() << newPoint.x() << newPoint.y() << endl;
    return newPoint;
}

QPoint Shape::scale_Point(int x, int y, float s, QPoint point) {
    QPoint newPoint;
    newPoint.setX((int)((float)0.5 + (float)x + (point.x() - x) * s));
    newPoint.setY((int)((float)0.5 + (float)y + (point.y() - y) * s));
    return newPoint;
}

int Shape::getType() {
    return type;
}

QPoint Shape::getCenter() {
    return transformCenter;
}

QPoint Shape::getLT() {
    return LTPoint;
}
QPoint Shape::getRB() {
    return RBPoint;
}

void Shape::clip(QPoint point1, QPoint point2, std::string algorithm) {
    return;
}

bool Shape::in_DraggingArea(QPoint cursor) {
    int xmin = LTPoint.x();
    int xmax = RBPoint.x();
    int ymin = LTPoint.y();
    int ymax = RBPoint.y();
    if((cursor.x() - xmin) * (cursor.x() - xmax) <= 0 &&
       (cursor.y() - ymin) * (cursor.y() - ymax) <= 0)
        return true;
    return false;
}
