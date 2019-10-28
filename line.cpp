#include "line.h"
#include <cmath>
#include <QDebug>

Line::Line() {
    type = LINE;
    drawMethod = 0;
    //0 for DDA, 1 for Bresenham
}

Line::Line(QPoint startPoint, QPoint endPoint) {
    type = LINE;
    drawMethod = 0;
    this->startPoint = startPoint;
    this->endPoint = endPoint;
    this->midPoint.setX((startPoint.x() + endPoint.x()) / 2);
    this->midPoint.setY((startPoint.y() + endPoint.y()) / 2);
}

void Line::setDrawMethod(int m) {
    drawMethod = m;
}

void Line::draw(QPen& pen, QPixmap& pix) {
    double deltaX = startPoint.x() - endPoint.x();
    double deltaY = startPoint.y() - endPoint.y();
    double k;
    if(fabs(deltaX) < 1e-5)
        k = 100000.0;
    else
        k = deltaY / deltaX;
    if(drawMethod == 0) {
        // DDA algorithm
        double length;
        if(fabs(k) > 1)
            length = fabs(deltaY);
        else
            length = fabs(deltaX);
        double stepX = deltaX / length;
        double stepY = deltaY / length;
        int x = endPoint.x(), y = endPoint.y();

        set_Pixel(x, y, pen, pix);
        for(int i = 1; i <= length; i++) {
            x += stepX;
            y += stepY;
            set_Pixel((int)(x + 0.5), (int)(y + 0.5), pen, pix);
        }
    }
    else {
        // Bresenham algorithm
        if(fabs(k) < 1.0) {
            if(deltaX * deltaY > 0) {
                int x0, x1, y0, y1;
                if(startPoint.x() < endPoint.x()) {
                    x0 = startPoint.x();
                    x1 = endPoint.x();
                    y0 = startPoint.y();
                    y1 = endPoint.y();
                }
                else {
                    x0 = endPoint.x();
                    y0 = endPoint.y();
                    x1 = startPoint.x();
                    y1 = startPoint.y();
                }
                int a = y0 - y1,b = x1 - x0, c = x0 * y1 - x1 * y0;
                int x = x0, y = y0;
                set_Pixel(x, y, pen, pix);
                while(x <= x1) {
                    if(2 * a * x + 2 * a + 2 * b * y + b + 2 * c < 0) {
                        x += 1;
                        y += 1;
                    }
                    else
                        x += 1;
                    set_Pixel(x, y, pen, pix);
                }
            }
            else {
                int x0, x1, y0, y1;
                if(startPoint.x() < endPoint.x()) {
                    x0 = startPoint.x();
                    x1 = endPoint.x();
                    y0 = startPoint.y();
                    y1 = endPoint.y();
                }
                else {
                    x0 = endPoint.x();
                    y0 = endPoint.y();
                    x1 = startPoint.x();
                    y1 = startPoint.y();
                }
                int a = y0 - y1, b = x1 - x0, c = x0 * y1 - x1 * y0;
                int x = x0, y = y0;
                set_Pixel(x, y, pen, pix);
                while(x <= x1) {
                    if(2 * a * x + 2 * a + 2 * b * y - b + 2 * c < 0)
                        x += 1;
                    else {
                        x += 1;
                        y -= 1;
                    }
                    set_Pixel(x, y, pen, pix);
                }
            }
        }
        else {
            if(deltaX * deltaY > 0) {
                int x0, x1, y0, y1;
                if(startPoint.y() < endPoint.y()) {
                    y0 = startPoint.y();
                    x0 = startPoint.x();
                    y1 = endPoint.y();
                    x1 = endPoint.x();
                }
                else {
                    y0 = endPoint.y();
                    x0 = endPoint.x();
                    y1 = startPoint.y();
                    x1 = startPoint.x();
                }
                int a = y0 - y1, b = x1 - x0, c = x0 * y1 - x1 * y0;
                int x = x0, y = y0;
                set_Pixel(x, y, pen, pix);
                while(y <= y1) {
                    if(2 * b * y + 2 * b + 2 * a * x + a + 2 * c < 0)
                        y += 1;
                    else {
                        x += 1;
                        y += 1;
                    }
                    set_Pixel(x, y, pen, pix);
                }
            }
            else {
                int x0, x1, y0, y1;
                if(startPoint.y() < endPoint.y()) {
                    y0 = startPoint.y();
                    x0 = startPoint.x();
                    y1 = endPoint.y();
                    x1 = endPoint.x();
                }
                else {
                    y0 = endPoint.y();
                    x0 = endPoint.x();
                    y1 = startPoint.y();
                    x1 = startPoint.x();
                }
                int a = y0 - y1, b = x1 - x0, c = x0 * y1 - x1 * y0;
                int x = x0, y = y0;
                set_Pixel(x, y, pen, pix);
                while(y <= y1) {
                    if(2 * b * y + 2 * b + 2 * a * x - a + 2 * c < 0) {
                        y += 1;
                        x -= 1;
                    }
                    else
                        y++;
                    set_Pixel(x, y, pen, pix);
                }
            }
        }
    }
}

void Line::set_Pixel(int x, int y, QPen& pen, QPixmap& pix) {
    QPainter painter(&pix);
    painter.setPen(pen);
    painter.drawPoint(x,y);
}

Line::~Line() {
    startPoint.setX(0);
    startPoint.setY(0);
    endPoint.setX(0);
    endPoint.setY(0);
    midPoint.setX(0);
    midPoint.setY(0);
}

void Line::translate(int dx, int dy) {
    startPoint = translate_Point(dx, dy, startPoint);
    endPoint = translate_Point(dx, dy, endPoint);
    midPoint.setX((startPoint.x() + endPoint.x()) / 2);
    midPoint.setY((startPoint.y() + endPoint.y()) / 2);
}

void Line::rotate(int x, int y, int r) {
    startPoint = rotate_Point(x, y, r, startPoint);
    endPoint = rotate_Point(x, y, r, endPoint);
    midPoint.setX((startPoint.x() + endPoint.x()) / 2);
    midPoint.setY((startPoint.y() + endPoint.y()) / 2);
}

void Line::scale(int x, int y, float s) {
    startPoint = scale_Point(x, y, s, startPoint);
    endPoint = scale_Point(x, y, s, endPoint);
    midPoint.setX((startPoint.x() + endPoint.x()) / 2);
    midPoint.setY((startPoint.y() + endPoint.y()) / 2);
}

void Line::clip(int x1, int y1, int x2, int y2, std::string algorithm) {
    if(algorithm == "1") {

    }
    else {

    }
}
