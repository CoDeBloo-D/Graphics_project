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
public:
    Line();
    Line(QPoint startPoint,QPoint endPoint);
    ~Line();
    void setDrawMethod(int m);
    void draw(QPen& pen, QPixmap& pix) override;
    void translate(int dx, int dy);
    void rotate(int x, int y, int r);
    void scale(int x, int y, float s);
    void clip(int x1, int y1, int x2, int y2, std::string algorithm);
};

#endif // LINE_H
