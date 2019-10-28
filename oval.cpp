#include "oval.h"
#include <cmath>

Oval::Oval() {
    type = OVAL;
    rotateDegrees = 0;
}

Oval::Oval(QPoint point1, QPoint point2) {
    type = OVAL;
    center = QPoint((point1.x() + point2.x()) / 2, (point1.y() + point2.y()) / 2);
    rotateDegrees = 0;
}

Oval::Oval(QPoint center, int a, int b) {
    type = OVAL;
    this->center = center;
    this->a = a;
    this->b = b;
    rotateDegrees = 0;
}

void Oval::draw(QPen& pen, QPixmap& pix) {
    if(a == 0 && b == 0)
        return;
    int x = 0;
    int y = b;
    /*
    为了保证计算过程中只有整数出现，
    所以将参数p和增量都乘了4
    */
    int p_area1 = 4 * b * b + a * a - 4 * a * a * b;
    int delta1 = 8 * a * a + 12 * b * b - 8 * a * a * b;
    int delta2 = 12 * b * b;

    //area1
    set_Pixel(x, y, pen, pix);
    while(b * b * x <= a * a * y) {
        if(p_area1 >= 0) {
            x += 1;
            y -= 1;
            p_area1 += delta1;
            delta1 = delta1 + 8 * b * b + 8 * a * a;
            delta2 = delta2 + 8 * b * b;
        }
        else {
            x++;
            p_area1 += delta2;
            delta1 += 8 * b * b;
            delta2 += 8 * b * b;
        }
        set_Pixel(x, y, pen, pix);
    }

    //area2
    int p_area2 = 4 * b * b * x * x + 4 * b * b * x + b * b + 4 * a * a * y * y
                  - 8 * a * a * y + 4 * a * a - 4 * a * a * b * b;
    delta1 = 8 * b * b * x - 8 * a * a * y
             + 8 * b * b + 12 * a * a;
    delta2 = -8 * a * a * y + 12 * a * a;
    while(y >= 0) {
        if(p_area2 <= 0) {
            x += 1;
            y -= 1;
            p_area2 += delta1;
            delta1 = delta1 + 8 * b * b + 8 * a * a;
            delta2 = delta2 + 8 * a * a;
        }
        else {
            y -= 1;
            p_area2 += delta2;
            delta1 += 8 * a * a;
            delta2 += 8 * a * a;
        }
        set_Pixel(x, y, pen, pix);
    }
}

void Oval::set_Pixel(int x, int y, QPen &pen, QPixmap &pix) {
    QPainter painter(&pix);
    painter.setPen(pen);

    double rad = ((double)rotateDegrees * 3.14159265358) / 180;
    double COS = cos(rad);
    double SIN = sin(rad);
    int oldX = x;
    int oldY = y;

    x = (int)(oldX * COS - oldY * SIN);
    y = (int)(oldX * SIN + oldY * COS);
    painter.drawPoint(x + center.x(), y + center.y());
    painter.drawPoint(-x + center.x(), -y + center.y());

    oldX = -oldX;
    x = (int)(oldX * COS - oldY * SIN);
    y = (int)(oldX * SIN + oldY * COS);
    painter.drawPoint(x + center.x(), y + center.y());
    painter.drawPoint(-x + center.x(), -y + center.y());
}

Oval::~Oval() {
    center.setX(0);
    center.setY(0);
    a = 0;
    b = 0;
}

void Oval::translate(int dx, int dy) {
    center = translate_Point(dx, dy, center);
}

void Oval::rotate(int x, int y, int r) {
    rotateDegrees = (rotateDegrees + r) % 360;
    center = rotate_Point(x, y, r, center);
}

void Oval::scale(int x, int y, float s) {
    center = scale_Point(x, y, s, center);
    a = (int)(a * s);
    b = (int)(b * s);
}
