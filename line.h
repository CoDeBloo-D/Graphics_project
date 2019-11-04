#ifndef LINE_H
#define LINE_H

#include"shape.h"
#include<QPixmap>
#include<QPen>
#include<QPoint>
#include<QPainter>

class Line : public Shape
{
private:
    QPoint startPoint;
    QPoint endPoint;
    QPoint midPoint;
    int drawMethod;
    void set_Pixel(int x, int y, QPen& pen, QPixmap& pix);
    void set_Pixel(double x, double y, QPen& pen, QPixmap& pix);
public:
    Line();
    Line(QPoint startPoint,QPoint endPoint);
    ~Line();
    void setDrawMethod(int m);
    void draw(QPen& pen, QPixmap& pix) override;
    void translate(int dx, int dy) override;
    void rotate(int x, int y, int r) override;
    void scale(int x, int y, float s) override;
    void clip(QPoint point1, QPoint point2, std::string algorithm) override;
};

#endif // LINE_H
