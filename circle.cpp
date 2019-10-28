#include "circle.h"
#include <cmath>

Circle::Circle() {
    type = CIRCLE;
}

Circle::Circle(QPoint center, int radius) {
    type = CIRCLE;
    this->center = center;
    this->radius = radius;
}

Circle::Circle(QPoint point1, QPoint point2) {
    type = CIRCLE;
    int delta_x = point1.x() - point2.x();
    int delta_y = point1.y() - point2.y();
    this->radius = (abs(delta_x) < abs(delta_y)) ? (abs(delta_x) / 2) : (abs(delta_y) / 2);
    if(delta_x < 0)
        this->center.setX(radius + point1.x());
    else
        this->center.setX(-radius + point1.x());

    if(delta_y < 0)
        this->center.setY(radius + point1.y());
    else
        this->center.setY(-radius + point1.y());
}

void Circle::draw(QPen& pen, QPixmap& pix) {
    int x = 0;
    int y = radius;
    int e = 1 - radius;
    int delta1 = 5 - 2 * radius;
    int delta2 = 3;
    while(x <= y) {
        set_Pixel(x,y,pen,pix);
        if(e > 0) {
            x += 1;
            y -= 1;
            e += delta1;
            delta1 += 4;
            delta2 += 2;
        }
        else {
            x += 1;
            e += delta2;
            delta1 += 2;
            delta2 += 2;
        }
    }
}

void Circle::set_Pixel(int x, int y, QPen &pen, QPixmap &pix) {
    QPainter painter(&pix);
    painter.setPen(pen);
    painter.drawPoint(x + center.x(), y + center.y());
    painter.drawPoint(x + center.x(), -y + center.y());
    painter.drawPoint(-x + center.x(), y + center.y());
    painter.drawPoint(-x + center.x(), -y + center.y());
    painter.drawPoint(y + center.x(), x + center.y());
    painter.drawPoint(y + center.x(), -x + center.y());
    painter.drawPoint(-y + center.x(), x + center.y());
    painter.drawPoint(-y + center.x(), -x + center.y());
}

Circle::~Circle() {
    center.setX(0);
    center.setY(0);
    radius = 0;
}

void Circle::translate(int dx, int dy) {
    center = translate_Point(dx, dy, center);
}

void Circle::rotate(int x, int y, int r) {
    center = rotate_Point(x, y, r, center);
}

void Circle::scale(int x, int y, float s) {
    center = scale_Point(x, y, s, center);
    radius = radius * s;
}
