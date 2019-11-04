#include "batch.h"
#include "ui_batch.h"
#include <QFileDialog>
#include <QDebug>

Batch::Batch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Batch)
{
    ui->setupUi(this);
    setGeometry(600, 300, 640, 480);
    setFixedSize(640, 480);
    setWindowTitle("Batch Mode");
    setWindowIcon(QIcon(":/source/source/Icon-terminal.png"));
    ui->pushButton->setGeometry(0, 0, 640, 200);
    ui->pushButton->setStyleSheet("border-style:dotted; border-width:4px;"
                                  "border-color:rgb(255, 255, 255);"
                                  "color: black; background: rgb(255, 241, 57, 100);");
    ui->pushButton->setFont(QFont("Book Antiqua", 40, QFont::Light));
    ui->label->setGeometry(0, 200, 640, 280);
    ui->label->setStyleSheet("border-width:0px;"
                             "color: black; background: rgb(212, 242, 231);");
    ui->label->setFont(QFont("Book Antiqua", 14, QFont::Light));
    ui->label->setText("Log:");
    ui->label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    inkColor = QColor(0, 0, 0);
    pen.setWidth(1);
    pen.setColor(inkColor);
    pen.setStyle(Qt::SolidLine);
    pix = QPixmap(160, 100);
    pix.fill(Qt::white);
}

Batch::~Batch()
{
    delete ui;
}

void Batch::load_File() {
    QFileDialog qfd(this);
    qfd.setDefaultSuffix("txt");
    //QString fPath = qfd.getOpenFileName(this, tr("Open File"),
                    //"D:\\examples\\", tr("TXT(*.txt)"));
    QString fPath = qfd.getOpenFileName(this, tr("Open File"),
                        "./", tr("TXT(*.txt)"));
    ui->label->setText("Log:");
    std::string path = fPath.toStdString();
    ifstream f(path);
    Command c;
    while(f >> c) {
        parse_Command(c);
    }
}

void Batch::reset_Canvas(int width, int height) {
    if(width < 100 || width > 1000 || height < 100 || height > 1000)
        return;
    pix = QPixmap(width, height);
    pix.fill(Qt::white);
    //white background
    pix.scaled(this->width(),this->height());

    update();
}

void Batch::save_Canvas(std::string fname) {
    QString fPath = "./" + QString::fromStdString(fname) + ".bmp";
    pix.save(fPath);
    QString newInfo = ui->label->text() + "\n" + QString::fromStdString(fname) + ".bmp" + " saved succeessfully!";
    ui->label->setText(newInfo);
}

void Batch::clean_Canvas() {
    pix.fill(Qt::white);
}

void Batch::redraw_Shapes() {
    clean_Canvas();
    for(int i = 0; i < shapeQueue.size(); i++) {
        pen.setColor(shapeColor[i]);
        shapeQueue[i]->draw(pen, pix);
    }
    pen.setColor(inkColor);
}

int Batch::get_shapeSub(int id) {
    for(int i = 0; i < shapeID.size(); i++) {
        if(id == shapeID[i])
            return i;
    }
}

void Batch::parse_Command(Command& c) {
    if(!strcmp(c.argv[0], "saveCanvas")) {
        string fname(c.argv[1]);
        save_Canvas(fname);
    }
    else if(!strcmp(c.argv[0], "resetCanvas")) {
        clean_Canvas();
        int w = atoi(c.argv[1]);
        int h = atoi(c.argv[2]);
        reset_Canvas(w, h);
    }
    else if(!strcmp(c.argv[0], "setColor")) {
        int R = atoi(c.argv[1]);
        int G = atoi(c.argv[2]);
        int B = atoi(c.argv[3]);
        inkColor = QColor(R, G, B);
        pen.setColor(inkColor);
    }
    else if(!strcmp(c.argv[0], "drawLine")) {
        int id = atoi(c.argv[1]);
        QPoint s(atoi(c.argv[2]), atoi(c.argv[3]));
        QPoint e(atoi(c.argv[4]), atoi(c.argv[5]));
        Line *newLine = new Line(s, e);
        if(!strcmp(c.argv[6], "Bresenham"))
            newLine->setDrawMethod(1);
        shapeQueue.push_back(newLine);
        shapeColor.push_back(inkColor);
        shapeID.push_back(id);
        newLine->draw(pen, pix);
    }
    else if(!strcmp(c.argv[0], "drawPolygon")) {
        int id = atoi(c.argv[1]);
        int n = atoi(c.argv[2]);
        QVector<QPoint> v;
        for(int i = 0; i < n; i++) {
            QPoint p(atoi(c.argv[4 +  2 * i]), atoi(c.argv[5 + 2 * i]));
            v.push_back(p);
        }
        Polygon *newPolygon = new Polygon(v);
        if(!strcmp(c.argv[3], "Bresenham"))
            newPolygon->setDrawMethod(1);
        shapeQueue.push_back(newPolygon);
        shapeColor.push_back(inkColor);
        shapeID.push_back(id);
        newPolygon->draw(pen, pix);
    }
    else if(!strcmp(c.argv[0], "drawEllipse")) {
        int id = atoi(c.argv[1]);
        QPoint center(atoi(c.argv[2]), atoi(c.argv[3]));
        int a = atoi(c.argv[4]), b = atoi(c.argv[5]);
        Oval *newOval = new Oval(center, a, b);
        shapeQueue.push_back(newOval);
        shapeColor.push_back(inkColor);
        shapeID.push_back(id);
        newOval->draw(pen, pix);
    }
    else if(!strcmp(c.argv[0], "drawCurve")) {
        int id = atoi(c.argv[1]);
        int n = atoi(c.argv[2]);
        QVector<QPoint> v;
        for(int i = 0; i < n; i++) {
            QPoint p(atoi(c.argv[4 +  2 * i]), atoi(c.argv[5 + 2 * i]));
            v.push_back(p);
        }
        Curve *newCurve = new Curve(v);
        if(!strcmp(c.argv[3], "B-spline"))
            newCurve->setDrawMethod(1);
        shapeQueue.push_back(newCurve);
        shapeColor.push_back(inkColor);
        shapeID.push_back(id);
        newCurve->draw(pen, pix);
    }
    else if(!strcmp(c.argv[0], "translate")) {
        int id = atoi(c.argv[1]);
        int dx = atoi(c.argv[2]);
        int dy = atoi(c.argv[3]);
        int shapeSub = get_shapeSub(id);
        shapeQueue[shapeSub]->translate(dx, dy);
        redraw_Shapes();
    }
    else if(!strcmp(c.argv[0], "rotate")) {
        int id = atoi(c.argv[1]);
        int x = atoi(c.argv[2]);
        int y = atoi(c.argv[3]);
        int r = atoi(c.argv[4]);
        int shapeSub = get_shapeSub(id);
        shapeQueue[shapeSub]->rotate(x, y, r);
        redraw_Shapes();
    }
    else if(!strcmp(c.argv[0], "scale")) {
        int id = atoi(c.argv[1]);
        int x = atoi(c.argv[2]);
        int y = atoi(c.argv[3]);
        float s = (float)atof(c.argv[4]);
        int shapeSub = get_shapeSub(id);
        shapeQueue[shapeSub]->scale(x, y, s);
        redraw_Shapes();
    }
    else if(!strcmp(c.argv[0], "clip")) {
        int id = atoi(c.argv[1]);
        int x1 = atoi(c.argv[2]);
        int y1 = atoi(c.argv[3]);
        int x2 = atoi(c.argv[4]);
        int y2 = atoi(c.argv[5]);
        QPoint p1(x1, y1), p2(x2, y2);
        std::string algorithm(c.argv[6]);
        int shapeSub = get_shapeSub(id);
        shapeQueue[shapeSub]->clip(p1, p2, algorithm);
        redraw_Shapes();
    }
}

void Batch::on_pushButton_clicked() {
    load_File();
}
