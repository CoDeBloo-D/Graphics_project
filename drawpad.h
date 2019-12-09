#ifndef DRAWPAD_H
#define DRAWPAD_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QImage>
#include <QPoint>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMainWindow>
#include <QRubberBand>
#include <QVector>
#include "line.h"
#include "circle.h"
#include "oval.h"
#include "rectangle.h"
#include "polygon.h"
#include "curve.h"

namespace Ui {
class Drawpad;
}

class Drawpad : public QWidget
{
    Q_OBJECT

public:
    explicit Drawpad(QWidget *parent = nullptr);
    ~Drawpad();

public:
    void init_Pen();
    bool save_Canvas();
    void reset_Canvas(int width, int height);
    void clean_Canvas();
    void set_inkColor();
    void set_penWidth();
    void redraw_Shapes();
    void input_fromDialog();

    void Line_switch(bool flag, int method);
    void draw_Line(QPoint startPoint, QPoint endPoint);
    void Circle_switch(bool flag);
    void draw_Circle(QPoint point1, QPoint point2);
    void Rectangle_switch(bool flag, int method);
    void draw_Rectangle(QPoint point1, QPoint point2);
    void Oval_switch(bool flag);
    void draw_Oval(QPoint point1, QPoint point2);
    void Polygon_switch(bool flag, int method);
    void draw_Polygon(QVector<QPoint> points);
    void Curve_switch(bool flag, int method);
    void draw_Curve(QVector<QPoint> points);
    void drag_switch(bool flag);

private:
    void show_auxiliaryPoint(QPoint point);
    void show_auxiliaryRec(Shape *s);
    void finish_currentDrawing();
    void set_transformPoint();
    void set_transformPointInput();
    void delete_currentShape();
    void rotate_currentShape(int degrees);
    void rotate_currentShape90();
    void rotate_currentShape180();
    void rotate_currentShapeInput();
    void scale_currentShape();
    void cut_Line();

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);
    void wheelEvent(QWheelEvent* event);
signals:
    void postionChange(const QPoint& pos, const QPoint& point, const int& shapeIdx, const int& type);
    //void unsavedChange(const bool& flag);
    void quitFunctions();
private:
    //QVector<QPixmap> pixQueue;
    //int pixIdx;
    QPixmap pix;
    QVector<Shape*> shapeQueue;
    QVector<QPen> shapePen;
    QColor inkColor;
    QPen pen;
    int penWidth;
    int currentShapeIdx;
    int drawMethod_line;
    int drawMethod_curve;
    QPoint transformPoint;
    QPoint pressPoint;
    QPoint releasePoint;
    QPoint currentPos;
    QVector<QPoint> pressPoints;
    int delta_x;
    int delta_y;
    bool isDrawingLine;
    bool isDrawingCircle;
    bool isDrawingRectangle;
    bool isDrawingOval;
    bool isDrawingPolygon;
    bool isDrawingCurve;
    bool isReleased;
    bool isDraggingShape;
    bool allowDragging;
    bool isCuttingLine;
    bool isResizing;
    QString cutAlgorithm;
    bool choose_Shape(QPoint cursor);

private:
    Ui::Drawpad *ui;
};

#endif // DRAWPAD_H
