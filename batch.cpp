#include "batch.h"
#include "ui_batch.h"
#include <QFileDialog>

Batch::Batch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Batch)
{
    ui->setupUi(this);
}

Batch::~Batch()
{
    delete ui;
}

void Batch::load_File() {
    QFileDialog qfd(this);
    qfd.setDefaultSuffix("txt");
    QString fPath = qfd.getOpenFileName(this, tr("Open File"),
                    "D:\\", tr("TXT(*.txt)"));
    string path = fPath.toStdString();
    ifstream f(path);
    Command c;
    while(f >> c) {
        commandSet.push_back(c);
    }
}

void Batch::parse_Command(Command& c) {
    if(!strcmp(c.argv[0], "saveCanvas")) {
        //TODO
    }
    else if(!strcmp(c.argv[0], "resetCanvas")) {
        //TODO
    }
    else if(!strcmp(c.argv[0], "setColor")) {
        //TODO
    }
    else if(!strcmp(c.argv[0], "drawLine")) {
        //TODO
    }
    else if(!strcmp(c.argv[0], "drawPolygon")) {
        //TODO
    }
    else if(!strcmp(c.argv[0], "drawEllipse")) {
        //TODO
    }
    else if(!strcmp(c.argv[0], "drawCurve")) {
        //TODO
    }
    else if(!strcmp(c.argv[0], "translate")) {
        //TODO
    }
    else if(!strcmp(c.argv[0], "rotate")) {
        //TODO
    }
    else if(!strcmp(c.argv[0], "scale")) {
        //TODO
    }
    else if(!strcmp(c.argv[0], "clip")) {
        //TODO
    }
}

void Batch::on_pushButton_clicked() {
    load_File();
}
