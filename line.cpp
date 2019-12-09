#include "line.h"
#include <cmath>
#include <QDebug>
#include <iostream>
#include <algorithm>

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
    set_LTRB();
}

void Line::setDrawMethod(int m) {
    drawMethod = m;
}

void Line::set_LTRB() {
    if(startPoint.x() < endPoint.x()) {
        LTPoint.setX(startPoint.x());
        RBPoint.setX(endPoint.x());
    }
    else if(startPoint.x() > endPoint.x()) {
        LTPoint.setX(endPoint.x());
        RBPoint.setX(startPoint.x());
    }
    else {
        LTPoint.setX(startPoint.x());
        RBPoint.setX(startPoint.x());
    }
    if(startPoint.y() < endPoint.y()) {
        LTPoint.setY(startPoint.y());
        RBPoint.setY(endPoint.y());
    }
    else if(startPoint.y() > endPoint.y()) {
        LTPoint.setY(endPoint.y());
        RBPoint.setY(startPoint.y());
    }
    else {
        LTPoint.setY(startPoint.y());
        RBPoint.setY(startPoint.y());
    }
    transformCenter.setX((startPoint.x() + endPoint.x()) / 2);
    transformCenter.setY((startPoint.y() + endPoint.y()) / 2);
}

void Line::draw(QPen& pen, QPixmap& pix) {
    if(startPoint == endPoint)
        return;
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
        double x = endPoint.x(), y = endPoint.y();
        set_Pixel(x, y, pen, pix);
        for(int i = 1; i <= length; i++) {
            x += stepX;
            y += stepY;
            set_Pixel(x, y, pen, pix);
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

void Line::set_Pixel(double x, double y, QPen& pen, QPixmap& pix) {
    QPainter painter(&pix);
    painter.setPen(pen);
    painter.drawPoint(QPointF(x, y));
}

Line::~Line() {
    startPoint.setX(0);
    startPoint.setY(0);
    endPoint.setX(0);
    endPoint.setY(0);
    transformCenter.setX(0);
    transformCenter.setY(0);
}

void Line::translate(int dx, int dy) {
    startPoint = translate_Point(dx, dy, startPoint);
    endPoint = translate_Point(dx, dy, endPoint);
    set_LTRB();
}

void Line::rotate(int x, int y, int r) {
    startPoint = rotate_Point(x, y, r, startPoint);
    endPoint = rotate_Point(x, y, r, endPoint);
    set_LTRB();
}

void Line::scale(int x, int y, float s) {
    startPoint = scale_Point(x, y, s, startPoint);
    endPoint = scale_Point(x, y, s, endPoint);
    set_LTRB();
}

int getAreaCode(int x, int y, int xmin, int xmax, int ymin, int ymax) {
    int areaCode = 0;
    if(x < xmin)
        areaCode |= 1;
    if(x>xmax)
        areaCode |= 2;
    if(y<ymin)
        areaCode |= 4;
    if(y>ymax)
        areaCode |= 8;

    return areaCode;
}

bool judgeAreaCode(int areaCode1, int areaCode2) {
    bool isDone = false;
    if(areaCode1 == 0 && areaCode2== 0)
        isDone = true;
    if((areaCode1&areaCode2) != 0)
        isDone = true;
    return isDone;
}

bool ClipT(int p, int q, double *u1, double *u2) {
    double r;
    if (p < 0) {
        r = (double)q / p;
        if (r > *u2)
            return false;
        if (r > *u1)
            *u1 = r;
    }
    else if (p > 0) {
        r = (double)q / p;
        if (r < *u1)
            return false;
        if (r < *u2)
            *u2 = r;
    }
    else if (q < 0)
        return false;
    return true;
}

void Line::clip(QPoint point1, QPoint point2, std::string algorithm) {
    int xmin, xmax, ymin, ymax;

    if(point1.x()>point2.x()) {
        xmin = point2.x();
        xmax = point1.x();
    }
    else {
        xmin = point1.x();
        xmax = point2.x();
    }
    if(point1.y()>point2.y()) {
        ymin = point2.y();
        ymax = point1.y();
    }
    else {
        ymin = point1.y();
        ymax = point2.y();
    }
    int x1 = startPoint.x(), y1 = startPoint.y();
    int x2 = endPoint.x(), y2 = endPoint.y();

    if(algorithm == "Cohen-Sutherland") {
        int areaCode1 = getAreaCode(x1, y1, xmin, xmax, ymin, ymax);
        int areaCode2 = getAreaCode(x2, y2, xmin, xmax, ymin, ymax);

        if((areaCode1 & areaCode2) != 0)
            return;
        if(areaCode1 == 0 && areaCode2 == 0) {
            endPoint = startPoint; //means the line is fully cut
            set_LTRB();
            return;
        }

        int flag1 = areaCode1 & 1, flag2 = areaCode2 & 1;
        int a = y2 - y1, b= x1 - x2, c = x2 * y1 - x1 * y2;

        //left cutting
        if(flag1 == 1) {
            x1 = xmin;
            y1 = (-c - a * x1) / b;
            areaCode1 = getAreaCode(x1, y1, xmin, xmax, ymin, ymax);
            bool isDone = judgeAreaCode(areaCode1, areaCode2);
            if(isDone) {
                startPoint.setX(x1);
                startPoint.setY(y1);
                endPoint.setX(x2);
                endPoint.setY(y2);
                set_LTRB();
                return;
            }
        }

        if(flag2 == 1) {
            x2 = xmin;
            y2 = (-c - a * x2) / b;
            areaCode2 = getAreaCode(x2, y2, xmin, xmax, ymin, ymax);
            bool isDone = judgeAreaCode(areaCode1, areaCode2);
            if(isDone) {
                startPoint.setX(x1);
                startPoint.setY(y1);
                endPoint.setX(x2);
                endPoint.setY(y2);
                set_LTRB();
                return;
            }
        }

        flag1 = (areaCode1 & 2) / 2;
        flag2 = (areaCode2 & 2) / 2;
        //right cutting
        if(flag1 == 1) {
            x1 = xmax;
            y1 = (-c - a * x1) / b;
            areaCode1 = getAreaCode(x1, y1, xmin, xmax, ymin, ymax);
            bool isDone = judgeAreaCode(areaCode1, areaCode2);
            if(isDone) {
                startPoint.setX(x1);
                startPoint.setY(y1);
                endPoint.setX(x2);
                endPoint.setY(y2);
                set_LTRB();
                return;
            }
        }

        if(flag2 == 1) {
            x2 = xmax;
            y2 = (-c - a * x2) / b;
            areaCode2 = getAreaCode(x2, y2, xmin, xmax, ymin, ymax);
            bool isDone = judgeAreaCode(areaCode1, areaCode2);
            if(isDone) {
                startPoint.setX(x1);
                startPoint.setY(y1);
                endPoint.setX(x2);
                endPoint.setY(y2);
                set_LTRB();
                return;
            }
        }

        flag1 = (areaCode1 & 4) / 4;
        flag2 = (areaCode2 & 4) / 4;
        //top cutting
        if(flag1 == 1) {
            y1 = ymin;
            x1 = (-c - b * y1) / a;
            areaCode1 = getAreaCode(x1, y1, xmin, xmax, ymin, ymax);
            bool isDone = judgeAreaCode(areaCode1, areaCode2);
            if(isDone) {
                startPoint.setX(x1);
                startPoint.setY(y1);
                endPoint.setX(x2);
                endPoint.setY(y2);
                set_LTRB();
                return;
            }
        }

        if(flag2 == 1) {
            y2 = ymin;
            x2 = (-c - b * y2) / a;
            areaCode2 = getAreaCode(x2, y2, xmin, xmax, ymin, ymax);
            bool isDone = judgeAreaCode(areaCode1, areaCode2);
            if(isDone) {
                startPoint.setX(x1);
                startPoint.setY(y1);
                endPoint.setX(x2);
                endPoint.setY(y2);
                set_LTRB();
                return;
            }
        }

        flag1 = (areaCode1 & 8) / 8;
        flag2 = (areaCode2 & 8) / 8;
        //bottom cutting
        if(flag1 == 1) {
            y1 = ymax;
            x1 = (-c - b * y1) / a;
            areaCode1 = getAreaCode(x1, y1, xmin, xmax, ymin, ymax);
            bool isDone = judgeAreaCode(areaCode1, areaCode2);
            if(isDone) {
                startPoint.setX(x1);
                startPoint.setY(y1);
                endPoint.setX(x2);
                endPoint.setY(y2);
                set_LTRB();
                return;
            }
        }

        if(flag2 == 1) {
            y2 = ymax;
            x2 = (-c - b * y2) / a;
            areaCode2 = getAreaCode(x2, y2, xmin, xmax, ymin, ymax);
            bool isDone = judgeAreaCode(areaCode1, areaCode2);
            if(isDone) {
                startPoint.setX(x1);
                startPoint.setY(y1);
                endPoint.setX(x2);
                endPoint.setY(y2);
                set_LTRB();
                return;
            }
        }

    }
    else {
        // algorithm == "Liang-Barsky"
        int dx = x2 - x1, dy = y2 - y1;
        double u1 = 0.0, u2 = 1.0;
        if (ClipT(-dx, x1 - xmin, &u1, &u2)) {
            if (ClipT(dx, xmax - x1, &u1, &u2)) {
                if (ClipT(-dy, y1 - ymin, &u1, &u2)) {
                    if (ClipT(dy, ymax - y1, &u1, &u2)) {
                        if (u2 < 1.0) {
                            x2 = (int)((double)x1 + u2 * dx + 0.5);
                            y2 = (int)((double)y1 + u2 * dy + 0.5);
                        }
                        if (u1 > 0.0) {
                            x1 = (int)((double)x1 + u1 * dx + 0.5);
                            y1 = (int)((double)y1 + u1 * dy + 0.5);
                        }
                        startPoint.setX(x1);
                        startPoint.setY(y1);
                        endPoint.setX(x2);
                        endPoint.setY(y2);
                        set_LTRB();
                    }
                    else
                        endPoint = startPoint;
                }
                else
                    endPoint = startPoint;
            }
            else
                endPoint = startPoint;
        }
        else
            endPoint = startPoint;
        set_LTRB();
    }

}
