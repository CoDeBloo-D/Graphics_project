#include "canvas.h"
#include "ui_canvas.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QColorDialog>
#include <QCursor>
#include <QImage>
#include <cmath>

Canvas::Canvas(QWidget *parent) : QWidget(parent), ui(new Ui::Canvas) {
    ui->setupUi(this);
    setStyleSheet("background: silver");
    setFixedSize(1200, 1100);
    pix = QPixmap(600, 450);
    pix.fill(Qt::white);
    //white background
    pix.scaled(this->width(),this->height());
    pixIdx = 0;
    pixQueue.push_back(pix);

    inkColor.setRgb(0, 0, 0);
    //black pen
    penWidth = 2;
    penStyle = 0;
    init_Pen();
}

Canvas::~Canvas() {
    pixQueue.clear();
    delete ui;
}

void Canvas::init_Pen() {
    pen.setColor(inkColor);
    pen.setWidth(penWidth);
    //pen.setStyle(edgeStyle);
    switch (penStyle) {
    case 0:
        pen.setStyle(Qt::SolidLine);
        break;
    case 1:
        pen.setStyle(Qt::DashLine);
        break;
    case 2:
        pen.setStyle(Qt::DotLine);
        break;
    default:
        pen.setStyle(Qt::SolidLine);
        break;
    }
}

void Canvas::load_Canvas(QString path) {
    QImage image(path);
    pix = QPixmap(600, 450);
    pix.fill(Qt::white);    //fill with white
    pix.scaled(this->width(),this->height());
    int width = this->width();
    int height = this->height();
    if(image.width()>width)
        image = image.scaled(width,image.height() * width / image.width());

    if(image.height()>height)
        image = image.scaled(image.width() * height / image.height(),height);

    QPainter painter(&pix);
    painter.drawImage(0,0,image);
    update();
    pixQueue.pop_back();
    pixQueue.push_back(pix); //save the initial canvas
}

bool Canvas::save_Canvas() {
    QFileDialog qfd(this);
    qfd.setDefaultSuffix("bmp");
    QString fPath = qfd.getSaveFileName(this, tr("Save File"),
                    "D:\\MyCanvas.bmp", tr("BMP(*.bmp)"));

    if(!fPath.isEmpty()) {
        QFile file(fPath);
        if(!(fPath.contains(".jpg") | (fPath.contains(".png")
             | fPath.contains(".jpeg")) | fPath.contains(".bmp"))) {
            QMessageBox::warning(this, tr("Save file"),
                                 tr("Save failed, please check file format(.jpg, .jpeg, .png, .bmp)"));
            return false;
        }

        pix.save(fPath);
        QMessageBox::information(this,tr("Information"),tr("Save succeed!"));
        return true;
    }
    else
        return false;
}

void Canvas::paintEvent(QPaintEvent *) {
    QPainter Painter(this);
    Painter.drawPixmap(0, 0, pix.width(), pix.height(), pix);
    update();
}

void Canvas::reset_Canvas(int width, int height) {
    if(width < 100 || width > 1000 || height < 100 || height > 1000) {
        QMessageBox::information(this,tr("Information"),tr("Illegal canvas size!"));
        return;
    }
    pix = QPixmap(width, height);
    pix.fill(Qt::white);
    //white background
    pix.scaled(this->width(),this->height());

    update();
}

void Canvas::set_inkColor() {
    QColorDialog askForColor;
    inkColor = askForColor.getColor();
    init_Pen();
}

void Canvas::on_Load_clicked()
{
    QFileDialog qfd(this);
    qfd.setDefaultSuffix("bmp");
    QString fPath = qfd.getOpenFileName(this, tr("Open File"),
                    "D:\\", tr("BMP(*.bmp)"));
    load_Canvas(fPath);
}

void Canvas::on_Reset_clicked()
{
    reset_Canvas(1000, 1000);
}

void Canvas::on_Color_clicked()
{
    set_inkColor();
}

void Canvas::on_Save_clicked()
{
    save_Canvas();
}

void Canvas::on_Test_clicked()
{
    Line *newLine = new Line(QPoint(0, 0), QPoint(200, 200));
    newLine->draw(pen, pix);
    //newLine->translate(100, 0);
    //newLine->draw(pen, pix);
    newLine->rotate(200, 200, 180);
    newLine->draw(pen, pix);
    //newLine->scale(0, 0, 2);
    //newLine->draw(pen, pix);
    //set_inkColor();
    /*
    Circle *newCircle = new Circle(QPoint(200, 200), 150);
    newCircle->draw(pen, pix);
    set_inkColor();
    Oval *newOval = new Oval(QPoint(200, 200), 150, 100);
    newOval->draw(pen, pix);
    set_inkColor();
    Rectangle *rec = new Rectangle(QPoint(0, 100), QPoint(400, 400));
    rec->draw(pen, pix);
    set_inkColor();
    QVector<QPoint> points;
    points.push_back(QPoint(300, 300));
    points.push_back(QPoint(300, 400));
    points.push_back(QPoint(400, 450));
    Polygon *polygon = new Polygon(points);
    polygon->draw(pen, pix);
    */
}
