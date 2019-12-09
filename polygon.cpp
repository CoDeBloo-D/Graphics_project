#include "polygon.h"

Polygon::Polygon() {
    type = POLYGON;
    drawMethod = 0;
    //0 for DDA, 1 for Bresenham
}

Polygon::Polygon(QVector<QPoint> v) {
    type = POLYGON;
    drawMethod = 0;
    for(int i = 0; i < v.size(); i++)
        points.push_back(v[i]);
    set_LTRB();
}

void Polygon::set_LTRB() {
    int minY, maxY, minX, maxX;
    minY = points[0].y();
    maxY = points[0].y();
    minX = points[0].x();
    maxX = points[0].x();
    for(int i = 0; i < points.size(); i++) {
        if(points[i].x() < minX)
            minX = points[i].x();
        if(points[i].x() > maxX)
            maxX = points[i].x();
        if(points[i].y() < minY)
            minY = points[i].y();
        if(points[i].y() > maxY)
            maxY = points[i].y();
    }
    LTPoint.setX(minX);
    LTPoint.setY(minY);
    RBPoint.setX(maxX);
    RBPoint.setY(maxY);
    transformCenter.setX((minX + maxX) / 2);
    transformCenter.setY((minY + maxY) / 2);
}

void Polygon::setDrawMethod(int m) {
    drawMethod = m;
}

void Polygon::draw(QPen &pen, QPixmap &pix) {
    for(int i = 0; i < points.size() - 1; i++) {
        Line *newLine = new Line(points[i], points[i + 1]);
        newLine->setDrawMethod(drawMethod);
        newLine->draw(pen, pix);
        delete newLine;
    }
    Line *newLine = new Line(points[points.size() - 1], points[0]);
    newLine->setDrawMethod(drawMethod);
    newLine->draw(pen, pix);
    delete newLine;
}

Polygon::~Polygon() {
    points.clear();
}

void Polygon::translate(int dx, int dy) {
    for(int i = 0; i < points.size(); i++)
        points[i] = translate_Point(dx, dy, points[i]);
    set_LTRB();
}

void Polygon::rotate(int x, int y, int r) {
    for(int i = 0; i < points.size(); i++)
        points[i] = rotate_Point(x, y, r, points[i]);
    set_LTRB();
}

void Polygon::scale(int x, int y, float s) {
    for(int i = 0; i < points.size(); i++)
        points[i] = scale_Point(x, y, s, points[i]);
    set_LTRB();
}
