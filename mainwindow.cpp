#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controlpanel.h"
#include <QIcon>
#include <QPalette>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setGeometry(600, 300, 640, 480);
    setFixedSize(640, 480);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/source/source/Icon-brush.png"));

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/source/source/background.png")));
    setPalette(palette);

    ui->pushButton->setGeometry(245, 165, 150, 150);
    ui->pushButton_3->setGeometry(600, 0, 40, 40);

    ui->pushButton->setStyleSheet("border-style:outset; border-width:4px; border-radius:10px; "
                                  "border-color:rgb(255, 255, 255, 200);"
                                  "color: black; background: rgb(230, 230, 230, 200);");
    ui->pushButton->setFont(QFont("Book Antiqua", 16, QFont::Light));
    QIcon icon(":/source/source/exit.png");
    ui->pushButton_3->setIcon(icon);
    ui->pushButton_3->setIconSize(QSize(40, 40));
    ui->pushButton_3->setText("");
    ui->pushButton_3->setStyleSheet("background: transparent; border-width: 0");
    lastPoint.setX(600);
    lastPoint.setY(300);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    cp = new Controlpanel;
    cp->exec();
}

void MainWindow::on_pushButton_3_clicked() {
    QMessageBox message(QMessageBox::Information, "Message", "Terminate drawing program?", QMessageBox::Yes | QMessageBox::No, NULL);
    if(message.exec()==QMessageBox::Yes)
        close();
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    lastPoint = event->globalPos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->globalX() - lastPoint.x();
    int dy = event->globalY() - lastPoint.y();
    lastPoint = event->globalPos();
    move(x() + dx, y() + dy);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    int dx = event->globalX() - lastPoint.x();
    int dy = event->globalY() - lastPoint.y();
    move(x() + dx, y() + dy);
}
