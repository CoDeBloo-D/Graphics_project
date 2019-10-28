#ifndef BATCH_H
#define BATCH_H

#include <QWidget>
#include <QVector>
#include "command.h"
#include "shape.h"
#include "line.h"
#include "circle.h"
#include "rectangle.h"
#include "oval.h"
#include "polygon.h"
#include "curve.h"

namespace Ui {
class Batch;
}

class Batch : public QWidget
{
    Q_OBJECT
private:
    QVector<Command> commandSet;
public:
    explicit Batch(QWidget *parent = nullptr);
    ~Batch();
private slots:
    void on_pushButton_clicked();

private:
    void load_File();
    void parse_Command(Command& c);

private:
    QVector<Shape> shapeQueue;
    QVector<int> shapeID;
    QVector<QColor> shapeColor;
    QPixmap pix;
    QColor inkColor;
    QPen pen;
    int penWidth;
private:
    Ui::Batch *ui;

};

#endif // BATCH_H
