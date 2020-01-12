#include "curve.h"
#include <cmath>
#include <QDebug>
Curve::Curve() {
    type = CURVE;
    drawMethod = 0;
    //0 for Bezier, 1 for B-spline
}

Curve::Curve(QVector<QPoint> v) {
    type = CURVE;
    drawMethod = 0;
    for(int i = 0; i < v.size(); i++)
        points.push_back(v[i]);
    LTPoint = QPoint(0, 0);
    RBPoint = QPoint(0, 0);
}

Curve::~Curve() {
    points.clear();
}

void Curve::set_LTRB() {
}

void Curve::setDrawMethod(int m) {
    drawMethod = m;
}

void Curve::set_Pixel(double x, double y, QPen& pen, QPixmap& pix) {
    QPainter painter(&pix);
    painter.setPen(pen);
    QPointF p(x, y);
    painter.drawPoint(p);

    int x_lower = (int) x;
    int x_upper = (int) x + 1;
    int y_lower = (int) y;
    int y_upper = (int) y + 1;
    if(LTPoint != QPoint(0, 0)) {
        if(x_lower < LTPoint.x())
            LTPoint.setX(x_lower);
        if(y_lower < LTPoint.y())
            LTPoint.setY(y_lower);
    }
    else {
        LTPoint.setX(x_lower);
        LTPoint.setY(y_lower);
    }
    if(RBPoint != QPoint(0, 0)) {
        if(x_upper > RBPoint.x())
            RBPoint.setX(x_upper);
        if(y_upper > RBPoint.y())
            RBPoint.setY(y_upper);
    }
    else {
        RBPoint.setX(x_upper);
        RBPoint.setY(y_upper);
    }
}

long factorial(long number) {
    if(number <= 1)
        return 1;
    else
        return number * factorial(number - 1);
}

int comb(int n, int m) {
    int temp;
    if(n < m) {
        temp = n;
        n = m;
        m = temp;
    }
    return (int)(factorial(n) / (factorial(m) * factorial(n-m)));
}

QPointF DeBoor(int j, int k, int i, double t, QVector<QPoint> controlPoints, double *knots) {
    if (j == 0)
        return controlPoints[i];
    else {
        double param = (t - knots[i]) / (knots[i + k - j] - knots[i]);
        return ((1-param) * DeBoor(j - 1, k, i - 1, t, controlPoints, knots)
                + param * DeBoor(j - 1, k, i, t, controlPoints, knots));
    }
}

int whichInterval(double t, double *knots, int length) {
    for (int i = 1; i <length - 1; i++) {
        if (t < knots[i])
            return (i - 1);
        else if (fabs(t - knots[length - 1]) < 1e-5)
            return (length - 1);
    }
    return -1;
}

void Curve::draw(QPen& pen, QPixmap& pix) {
    if(drawMethod == 0) {
        //Bezier
        double x, y;
        for(int t = 0; t <= 1000; t++) {
            x = 0.0;
            y = 0.0;
            for(int i = 0; i < points.size(); i++) {
                x += comb(points.size() - 1, i) * points[i].x() * pow((1 - 0.001 * t), (points.size() - 1 - i)) * pow(0.001 * t, i);
                y += comb(points.size() - 1, i) * points[i].y() * pow((1 - 0.001 * t), (points.size() - 1 - i)) * pow(0.001 * t, i);
            }
            set_Pixel(x, y, pen, pix);
        }
    }
    else {
        //B-spline
        int order = 3;
        int nKnots = order + points.size();
        double *knots = new double[nKnots];
        for (int i = 0; i < nKnots; i++) {
            if (i < order)
                knots[i] = 0;
            else if (i < nKnots - order + 1)
                knots[i] = knots[i - 1] + 1;
            else
                knots[i] = knots[i - 1];
        }
        double stepT = (knots[nKnots - order] - knots[order - 1]) / (1000 - 1);
        double t;
        QPointF p;
        for (int i = 0; i < 1000; i++) {
            if (i == 999) {
                p = points[points.size() - 1];
                set_Pixel(p.x(), p.y(), pen, pix);
            }
            else {
                t = knots[order - 1] + i * stepT;
                int tInt = whichInterval(t, knots, nKnots);
                if (tInt >= points.size())
                    continue;
                p= DeBoor(order - 1, order, tInt, t, points, knots);
                set_Pixel(p.x(), p.y(), pen, pix);
            }
        }
    }
    transformCenter.setX((LTPoint.x() + RBPoint.x()) / 2);
    transformCenter.setY((LTPoint.y() + RBPoint.y()) / 2);
}

void Curve::translate(int dx, int dy) {
    for(int i = 0; i < points.size(); i++)
        points[i] = translate_Point(dx, dy, points[i]);
    LTPoint = RBPoint = QPoint(0, 0);
}

void Curve::rotate(int x, int y, int r) {
    for(int i = 0; i < points.size(); i++)
        points[i] = rotate_Point(x, y, r, points[i]);
    LTPoint = RBPoint = QPoint(0, 0);
}

void Curve::scale(int x, int y, float s) {
    for(int i = 0; i < points.size(); i++)
        points[i] = scale_Point(x, y, s, points[i]);
    LTPoint = RBPoint = QPoint(0, 0);
}
