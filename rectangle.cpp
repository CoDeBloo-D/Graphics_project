#include "rectangle.h"

Rectangle::Rectangle() {
    point1 = QPoint(0,0);
    point2 = QPoint(0,0);
    type = RECTANGLE;
    drawMethod = 0;
}

Rectangle::Rectangle(QPoint point1,QPoint point2){
    type = RECTANGLE;
    this->point1 = point1;
    this->point2 = point2;
    point3 = QPoint(point1.x(),point2.y());
    point4 = QPoint(point2.x(),point1.y());
    center.setX((point1.x()+point2.x())/2);
    center.setY((point1.y()+point2.y())/2);

    QPoint temp = this->point2;
    this->point2 = point4;
    this->point4 = point3;
    this->point3 = temp;
    drawMethod = 0;
}

void Rectangle::setDrawMethod(int m) {
    drawMethod = m;
    line1.setDrawMethod(m);
    line2.setDrawMethod(m);
    line3.setDrawMethod(m);
    line4.setDrawMethod(m);
}

void Rectangle::draw(QPen& pen, QPixmap& pix) {
    line1 = Line(point1,point2);
    line2 = Line(point2,point3);
    line3 = Line(point3,point4);
    line4 = Line(point4,point1);

    line1.draw(pen, pix);
    line2.draw(pen, pix);
    line3.draw(pen, pix);
    line4.draw(pen, pix);
}

Rectangle::~Rectangle() {
    point1.setX(0);
    point1.setY(0);
    point2.setX(0);
    point2.setY(0);
    point3.setX(0);
    point3.setY(0);
    point4.setX(0);
    point4.setY(0);
    line1.~Line();
    line2.~Line();
    line3.~Line();
    line4.~Line();
}

void Rectangle::translate(int dx, int dy) {
    point1 = translate_Point(dx, dy, point1);
    point2 = translate_Point(dx, dy, point2);
    point3 = translate_Point(dx, dy, point3);
    point4 = translate_Point(dx, dy, point4);
}

void Rectangle::rotate(int x, int y, int r) {
    point1 = rotate_Point(x, y, r, point1);
    point2 = rotate_Point(x, y, r, point2);
    point3 = rotate_Point(x, y, r, point3);
    point4 = rotate_Point(x, y, r, point4);
}

void Rectangle::scale(int x, int y, float s) {
    point1 = scale_Point(x, y, s, point1);
    point2 = scale_Point(x, y, s, point2);
    point3 = scale_Point(x, y, s, point3);
    point4 = scale_Point(x, y, s, point4);
}
