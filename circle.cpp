#include "circle.h"
#include <cmath>

Circle::Circle() {
    type = CIRCLE;
}

Circle::Circle(QPoint center, int radius) {
    type = CIRCLE;
    this->center = center;
    this->radius = radius;
    transformCenter = center;
    set_LTRB();
}

Circle::Circle(QPoint point1, QPoint point2) {
    type = CIRCLE;
    center = QPoint((point1.x() + point2.x()) / 2, (point1.y() + point2.y()) / 2);
    int delta_x = point1.x() - point2.x(), delta_y = point1.y() - point2.y();
    radius = (int)(pow(delta_x * delta_x + delta_y * delta_y, 0.5)) / 2;
    set_LTRB();
}

void Circle::set_LTRB() {
    LTPoint.setX(center.x() - radius - 1);
    LTPoint.setY(center.y() - radius - 1);
    RBPoint.setX(center.x() + radius + 1);
    RBPoint.setY(center.y() + radius + 1);
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
    transformCenter = center;
    set_LTRB();
}

void Circle::rotate(int x, int y, int r) {
    center = rotate_Point(x, y, r, center);
    transformCenter = center;
    set_LTRB();
}

void Circle::scale(int x, int y, float s) {
    center = scale_Point(x, y, s, center);
    radius = radius * s;
    transformCenter = center;
    set_LTRB();
}
