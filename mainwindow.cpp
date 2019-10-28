#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"
#include "batch.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    Canvas *c = new Canvas;
    c->show();

}

void MainWindow::on_pushButton_2_clicked()
{
    Batch *b = new Batch;
    b->show();
}
