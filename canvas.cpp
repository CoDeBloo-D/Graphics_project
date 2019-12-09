#include "canvas.h"
#include "ui_canvas.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QInputDialog>
#include <QColorDialog>
#include <QCursor>
#include <QImage>
#include <cmath>

Canvas::Canvas(QWidget *parent) : QWidget(parent), ui(new Ui::Canvas) {
    ui->setupUi(this);
    setGeometry(300, 50, 1200, 1000);
    setFixedSize(1200, 1000);
    setWindowTitle("New Project");
    setWindowIcon(QIcon(":/source/source/Icon-mouse.png"));
    setStyleSheet("background: rgb(161, 175, 201)");
    setMouseTracking(true);
    ui->Load->setGeometry(1010, 50, 180, 40);
    ui->Save->setGeometry(1010, 100, 180, 40);
    ui->Reset->setGeometry(1010, 150, 180, 40);
    ui->Color->setGeometry(1010, 200, 180, 40);
    ui->Input->setGeometry(1010, 250, 180, 40);
    ui->Transform->setGeometry(1010, 300, 180, 40);
    ui->Load->setStyleSheet("border-style:solid; border-width:4px;"
                            "border-color:rgb(255, 255, 255);"
                            "color: black; background: rgb(242, 236, 222);");
    ui->Load->setFont(QFont("Book Antiqua", 16, QFont::Light));
    ui->Save->setStyleSheet("border-style:solid; border-width:4px;"
                            "border-color:rgb(255, 255, 255);"
                            "color: black; background: rgb(242, 236, 222);");
    ui->Save->setFont(QFont("Book Antiqua", 16, QFont::Light));
    ui->Reset->setStyleSheet("border-style:solid; border-width:4px;"
                            "border-color:rgb(255, 255, 255);"
                            "color: black; background: rgb(242, 236, 222);");
    ui->Reset->setFont(QFont("Book Antiqua", 16, QFont::Light));
    ui->Color->setStyleSheet("border-style:solid; border-width:4px;"
                            "border-color:rgb(255, 255, 255);"
                            "color: black; background: rgb(242, 236, 222);");
    ui->Color->setFont(QFont("Book Antiqua", 16, QFont::Light));
    ui->Input->setStyleSheet("border-style:solid; border-width:4px;"
                            "border-color:rgb(255, 255, 255);"
                            "color: black; background: rgb(242, 236, 222);");
    ui->Input->setFont(QFont("Book Antiqua", 16, QFont::Light));
    ui->Transform->setStyleSheet("border-style:solid; border-width:4px;"
                            "border-color:rgb(255, 255, 255);"
                            "color: black; background: rgb(242, 236, 222);");
    ui->Transform->setFont(QFont("Book Antiqua", 16, QFont::Light));
    pix = QPixmap(500, 500);
    pix.fill(Qt::white);

    inkColor.setRgb(0, 0, 0);
    penWidth = 2;
    penStyle = 0;
    init_Pen();
}

Canvas::~Canvas() {
    delete ui;
}

void Canvas::init_Pen() {
    pen.setColor(inkColor);
    pen.setWidth(penWidth);
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
    pix.fill(Qt::white);    //fill with white
    pix.scaled(this->width(),this->height());
    int width = this->width();
    int height = this->height();
    if(image.width() > width)
        image = image.scaled(width,image.height() * width / image.width());

    if(image.height() > height)
        image = image.scaled(image.width() * height / image.height(),height);

    QPainter painter(&pix);
    painter.drawImage(0,0,image);
    update();
}

bool Canvas::save_Canvas() {
    QFileDialog qfd(this);
    qfd.setDefaultSuffix("bmp");
    QString fPath = qfd.getSaveFileName(this, tr("Save File"),
                    "D:\\MyCanvas.png", tr("PNG(*.png)"));

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

void Canvas::redraw_Shapes() {
    pix.fill(Qt::white);
    for(int i = 0; i < shapeQueue.size(); i++) {
        pen.setColor(shapeColor[i]);
        shapeQueue[i]->draw(pen, pix);
    }
    pen.setColor(inkColor);
    update();
}

void Canvas::on_Load_clicked() {
    QFileDialog qfd(this);
    qfd.setDefaultSuffix("bmp");
    QString fPath = qfd.getOpenFileName(this, tr("Open File"),
                    "D:\\", tr("BMP(*.bmp)"));
    load_Canvas(fPath);
}

void Canvas::on_Reset_clicked() {
    bool flag1 = false, flag2 = false;
    int w = QInputDialog::getInt(nullptr, "Width",
                                "Width:", 100, 100, 1000, 1, &flag1);
    int h = QInputDialog::getInt(nullptr, "Height",
                                "Height:", 100, 100, 1000, 1, &flag2);
    if (flag1 && flag2)
        reset_Canvas(w, h);
}

void Canvas::on_Color_clicked() {
    set_inkColor();
}

void Canvas::on_Save_clicked() {
    save_Canvas();
}

void Canvas::on_Input_clicked() {
    QStringList list;
    list << "Line" << "Rectangle" << "Circle" << "Oval" << "Polygon" << "Curve";
    bool ok = false;
    QString res = QInputDialog::getItem(nullptr, "Shape List",
                                        "Please select a shape:", list, 0, true, &ok);
    if(!ok)
        return;

    if(res == "Line") {
        int x1 = QInputDialog::getInt(nullptr, "x1",
                                    "x1:", 0, 0, pix.width(), 1, nullptr);
        int y1 = QInputDialog::getInt(nullptr, "y1",
                                    "y1:", 0, 0, pix.height(), 1, nullptr);
        int x2 = QInputDialog::getInt(nullptr, "x2",
                                    "x2:", 0, 0, pix.width(), 1, nullptr);
        int y2 = QInputDialog::getInt(nullptr, "y2",
                                    "y2:", 0, 0, pix.height(), 1, nullptr);
        QPoint s(x1, y1), e(x2, y2);
        QStringList algorithmList;
        algorithmList << "DDA" << "Bresenham";
        QString algorithm = QInputDialog::getItem(nullptr, "Algorithm",
                                                  "Please select an algorithm:", algorithmList, 1, true, nullptr);
        Line *newLine = new Line(s, e);
        if(algorithm == "Bresenham")
            newLine->setDrawMethod(1);
        shapeQueue.push_back(newLine);
        shapeColor.push_back(inkColor);
        newLine->draw(pen, pix);
    }
    if(res == "Rectangle") {
        int x1 = QInputDialog::getInt(nullptr, "x1",
                                    "x1:", 0, 0, pix.width(), 1, nullptr);
        int y1 = QInputDialog::getInt(nullptr, "y1",
                                    "y1:", 0, 0, pix.height(), 1, nullptr);
        int x2 = QInputDialog::getInt(nullptr, "x2",
                                    "x2:", 0, 0, pix.width(), 1, nullptr);
        int y2 = QInputDialog::getInt(nullptr, "y2",
                                    "y2:", 0, 0, pix.height(), 1, nullptr);
        QStringList algorithmList;
        algorithmList << "DDA" << "Bresenham";
        QString algorithm = QInputDialog::getItem(nullptr, "Algorithm",
                                                  "Please select an algorithm:", algorithmList, 1, true, nullptr);
        QPoint LT(x1, y1);
        QPoint RB(x2, y2);
        Rectangle *newRectangle = new Rectangle(LT, RB);
        if(algorithm == "Bresenham")
            newRectangle->setDrawMethod(1);
        shapeQueue.push_back(newRectangle);
        shapeColor.push_back(inkColor);
        newRectangle->draw(pen, pix);
    }
    if(res == "Circle") {
        int x = QInputDialog::getInt(nullptr, "x",
                                    "x:", 0, 0, pix.width(), 1, nullptr);
        int y = QInputDialog::getInt(nullptr, "y",
                                    "y:", 0, 0, pix.height(), 1, nullptr);
        int r = QInputDialog::getInt(nullptr, "Radius", "r:", 0, 0,
                                     pix.width() < pix.height() ? pix.width() / 2 : pix.height() / 2, 1, nullptr);
        Circle *newCircle = new Circle(QPoint(x, y), r);
        shapeQueue.push_back(newCircle);
        shapeColor.push_back(inkColor);
        newCircle->draw(pen, pix);
    }
    if(res == "Oval") {
        int x = QInputDialog::getInt(nullptr, "x",
                                    "x:", 0, 0, pix.width(), 1, nullptr);
        int y = QInputDialog::getInt(nullptr, "y",
                                    "y:", 0, 0, pix.height(), 1, nullptr);
        int a = QInputDialog::getInt(nullptr, "a", "a:", 0, 0,
                                     pix.width() < pix.height() ? pix.width() / 2 : pix.height() / 2, 1, nullptr);
        int b = QInputDialog::getInt(nullptr, "b", "b:", 0, 0,
                                     pix.width() < pix.height() ? pix.width() / 2 : pix.height() / 2, 1, nullptr);
        Oval *newOval = new Oval(QPoint(x, y), a, b);
        shapeQueue.push_back(newOval);
        shapeColor.push_back(inkColor);
        newOval->draw(pen, pix);
    }
    if(res == "Polygon") {
        int n = QInputDialog::getInt(nullptr, "n",
                                    "the number of vertexes:", 3, 3, 20, 1, nullptr);
        QVector<QPoint> v;
        for(int i = 0; i < n; i++) {
            int x = QInputDialog::getInt(nullptr, "x",
                                    "x:", 0, 0, pix.width(), 1, nullptr);
            int y = QInputDialog::getInt(nullptr, "y",
                                    "y:", 0, 0, pix.height(), 1, nullptr);
            v.push_back(QPoint(x, y));
        }
        QStringList algorithmList;
        algorithmList << "DDA" << "Bresenham";
        QString algorithm = QInputDialog::getItem(nullptr, "Algorithm",
                                                  "Please select an algorithm:", algorithmList, 1, true, nullptr);
        Polygon *newPolygon = new Polygon(v);
        if(algorithm == "Bresenham")
            newPolygon->setDrawMethod(1);
        shapeQueue.push_back(newPolygon);
        shapeColor.push_back(inkColor);
        newPolygon->draw(pen, pix);
    }
    if(res == "Curve") {
        int n = QInputDialog::getInt(nullptr, "n",
                                    "the number of control points:", 0, 0, 20, 1, nullptr);
        QVector<QPoint> v;
        for(int i = 0; i < n; i++) {
            int x = QInputDialog::getInt(nullptr, "x",
                                    "x:", 0, 0, pix.width(), 1, nullptr);
            int y = QInputDialog::getInt(nullptr, "y",
                                    "y:", 0, 0, pix.height(), 1, nullptr);
            v.push_back(QPoint(x, y));
        }
        QStringList algorithmList;
        algorithmList << "Bezier" << "B-spline";
        QString algorithm = QInputDialog::getItem(nullptr, "Algorithm",
                                                  "Please select an algorithm:", algorithmList, 1, true, nullptr);
        Curve *newCurve = new Curve(v);
        if(algorithm == "B-spline")
            newCurve->setDrawMethod(1);
        shapeQueue.push_back(newCurve);
        shapeColor.push_back(inkColor);
        newCurve->draw(pen, pix);
    }
}

void Canvas::on_Transform_clicked() {
    QStringList list;
    list << "Translate" << "Rotate" << "Scale" << "Clip";
    bool ok = false;
    QString res = QInputDialog::getItem(nullptr, "Transformation",
                                        "Please select a transformation:", list, 0, true, &ok);
    if(!ok)
        return;

    if(res == "Translate") {
        int i = QInputDialog::getInt(nullptr, "i",
                                    "the id of the shape:", 0, 0, shapeQueue.size() - 1, 1, nullptr);
        int dx = QInputDialog::getInt(nullptr, "dx",
                                "dx:", 0, 0, pix.width(), 1, nullptr);
        int dy = QInputDialog::getInt(nullptr, "dy",
                                "dy:", 0, 0, pix.height(), 1, nullptr);
        shapeQueue[i]->translate(dx, dy);
        redraw_Shapes();
    }
    if(res == "Rotate") {
        int i = QInputDialog::getInt(nullptr, "i",
                                    "the id of the shape:", 0, 0, shapeQueue.size() - 1, 1, nullptr);
        int x = QInputDialog::getInt(nullptr, "x",
                                "x:", 0, 0, pix.width(), 1, nullptr);
        int y = QInputDialog::getInt(nullptr, "dy",
                                "y:", 0, 0, pix.height(), 1, nullptr);
        int r = QInputDialog::getInt(nullptr, "r",
                                "rotate degrees:", 0, 0, 360, 1, nullptr);
        shapeQueue[i]->rotate(x, y, r);
        redraw_Shapes();
    }
    if(res == "Scale") {
        int i = QInputDialog::getInt(nullptr, "i",
                                    "the id of the shape:", 0, 0, shapeQueue.size() - 1, 1, nullptr);
        int x = QInputDialog::getInt(nullptr, "x",
                                "x:", 0, 0, pix.width(), 1, nullptr);
        int y = QInputDialog::getInt(nullptr, "dy",
                                "y:", 0, 0, pix.height(), 1, nullptr);
        double s = QInputDialog::getDouble(nullptr, "scale", "scale:", 0, 0, 1000, 1, nullptr);
        shapeQueue[i]->scale(x, y, (float)s);
        redraw_Shapes();
    }
    if(res == "Clip") {
        int i = QInputDialog::getInt(nullptr, "i",
                                    "the id of the shape:", 0, 0, shapeQueue.size() - 1, 1, nullptr);
        int x1 = QInputDialog::getInt(nullptr, "x1",
                                    "x1:", 0, 0, pix.width(), 1, nullptr);
        int y1 = QInputDialog::getInt(nullptr, "y1",
                                    "y1:", 0, 0, pix.height(), 1, nullptr);
        int x2 = QInputDialog::getInt(nullptr, "x2",
                                    "x2:", 0, 0, pix.width(), 1, nullptr);
        int y2 = QInputDialog::getInt(nullptr, "y2",
                                    "y2:", 0, 0, pix.height(), 1, nullptr);
        QPoint LT(x1, y1);
        QPoint RB(x2, y2);
        QStringList algorithmList;
        algorithmList << "Cohen-Sutherland" << "Liang-Barsky";
        QString algorithm = QInputDialog::getItem(nullptr, "Algorithm",
                                                  "Please select an algorithm:", algorithmList, 1, true, nullptr);
        shapeQueue[i]->clip(LT, RB, algorithm.toStdString());
        redraw_Shapes();
    }
}
