#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QImage>
#include <QPoint>
#include <QPaintEvent>
#include <QFileDialog>
#include <QMainWindow>
#include <QRubberBand>
#include <QVector>
#include "line.h"
#include "circle.h"
#include "oval.h"
#include "rectangle.h"
#include "polygon.h"

namespace Ui {
class Canvas;
}

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas() override;
private slots:
    void on_Test_clicked();
    void on_Save_clicked();
    void on_Load_clicked();
    void on_Reset_clicked();
    void on_Color_clicked();

private:
    void init_Pen();
    void load_Canvas(QString path);
    bool save_Canvas();
    void reset_Canvas(int width, int height);
    void set_inkColor();
    void paintEvent(QPaintEvent *) override;

private:
    QVector<QPixmap>pixQueue;
    int pixIdx;
    QPixmap pix;      //buffer
    QColor inkColor;
    QPen pen;
    int penWidth;
    int penStyle;

private:
    Ui::Canvas *ui;

};

#endif // CANVAS_H
