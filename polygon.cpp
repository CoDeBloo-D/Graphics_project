#include "polygon.h"

Polygon::Polygon() {
    type = POLYGON;
}

Polygon::Polygon(QVector<QPoint> v) {
    type = POLYGON;
    for(int i = 0; i < v.size(); i++)
        points.push_back(v[i]);
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
}

void Polygon::rotate(int x, int y, int r) {
    for(int i = 0; i < points.size(); i++)
        points[i] = rotate_Point(x, y, r, points[i]);
}

void Polygon::scale(int x, int y, float s) {
    for(int i = 0; i < points.size(); i++)
        points[i] = scale_Point(x, y, s, points[i]);
}
