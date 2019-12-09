#include "controlpanel.h"
#include "ui_controlpanel.h"
#include <QInputDialog>
#include <QStatusBar>
#include <QToolBar>
#include <QHBoxLayout>
#include <QAction>

Controlpanel::Controlpanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Controlpanel)
{
    ui->setupUi(this);
    setWindowTitle(tr("New Project"));
    setWindowFlags(Qt::Window);
    setWindowIcon(QIcon(":/source/source/Icon-mouse.png"));
    mainWindow = new QMainWindow();
    mainWindow->statusBar();
    toolBar = new QToolBar();
    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(30,30));
    toolBar->setFixedHeight(40);
    mainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(mainWindow);
    layout->setMargin(10);
    setLayout(layout);
    drawpad = new Drawpad();
    drawpad->setGeometry(0, 0, 1000, 800);
    mainWindow->setCentralWidget(drawpad);
    mainWindow->setStyleSheet("background: rgb(230, 230, 230, 200)");

    drawLine = new QAction(QIcon(":/source/source/line.png"), tr("draw a line"), toolBar);
    drawLine->setStatusTip(tr("draw a line"));
    drawLine->setToolTip(tr("draw a line"));
    drawLine->setCheckable(true);
    connect(drawLine, &QAction::triggered, this, &Controlpanel::draw_Line);
    toolBar->addAction(drawLine);
    drawCircle = new QAction(QIcon(":/source/source/circle.png"), tr("draw a circle"), toolBar);
    drawCircle->setStatusTip(tr("draw a circle"));
    drawCircle->setToolTip(tr("draw a circle"));
    drawCircle->setCheckable(true);
    connect(drawCircle, &QAction::triggered, this, &Controlpanel::draw_Circle);
    toolBar->addAction(drawCircle);
    drawOval = new QAction(QIcon(":/source/source/oval.png"), tr("draw an oval"), toolBar);
    drawOval->setStatusTip(tr("draw an oval"));
    drawOval->setToolTip(tr("draw an oval"));
    drawOval->setCheckable(true);
    connect(drawOval, &QAction::triggered, this, &Controlpanel::draw_Oval);
    toolBar->addAction(drawOval);
    drawRectangle = new QAction(QIcon(":/source/source/rectangle.png"), tr("draw a rectangle"), toolBar);
    drawRectangle->setStatusTip(tr("draw a rectangle"));
    drawRectangle->setToolTip(tr("draw a rectangle"));
    drawRectangle->setCheckable(true);
    connect(drawRectangle,&QAction::triggered,this,&Controlpanel::draw_Rectangle);
    toolBar->addAction(drawRectangle);
    drawPolygon = new QAction(QIcon(":/source/source/polygon.png"), tr("draw a polygon"), toolBar);
    drawPolygon->setStatusTip(tr("draw a polygon"));
    drawPolygon->setToolTip(tr("draw a polygon"));
    drawPolygon->setCheckable(true);
    connect(drawPolygon, &QAction::triggered, this, &Controlpanel::draw_Polygon);
    toolBar->addAction(drawPolygon);
    drawCurve = new QAction(QIcon(":/source/source/curve.png"), tr("draw a curve"), toolBar);
    drawCurve->setStatusTip(tr("draw a curve"));
    drawCurve->setToolTip(tr("draw a curve"));
    drawCurve->setCheckable(true);
    connect(drawCurve,&QAction::triggered, this, &Controlpanel::draw_Curve);
    toolBar->addAction(drawCurve);
    drag = new QAction(QIcon(":/source/source/drag.png"), tr("allow dragging"), toolBar);
    drag->setStatusTip("allow dragging");
    drag->setToolTip("allow dragging");
    drag->setCheckable(true);
    connect(drag, &QAction::triggered, this, &Controlpanel::allow_dragging);
    toolBar->addAction(drag);
    setColor = new QAction(QIcon(":/source/source/color.png"), tr("set ink color"), toolBar);
    setColor->setStatusTip("set ink color");
    setColor->setToolTip("set ink color");
    connect(setColor, &QAction::triggered, this, &Controlpanel::set_inkColor);
    toolBar->addAction(setColor);
    setPen = new QAction(QIcon(":/source/source/pen.png"), tr("set pen style"), toolBar);
    setPen->setStatusTip("set pen width");
    setPen->setToolTip("set pen width");
    connect(setPen, &QAction::triggered, this, &Controlpanel::set_penWidth);
    toolBar->addAction(setPen);
    resize = new QAction(QIcon(":/source/source/resize.png"), tr("resize canvas"), toolBar);
    resize->setStatusTip("resize canvas");
    resize->setToolTip("resize canvas");
    connect(resize, &QAction::triggered, this, &Controlpanel::reset_Canvas);
    toolBar->addAction(resize);
    reset = new QAction(QIcon(":/source/source/reset.png"), tr("reset canvas"), toolBar);
    reset->setStatusTip("reset canvas");
    reset->setToolTip("reset canvas");
    connect(reset, &QAction::triggered, this, &Controlpanel::clean_Canvas);
    toolBar->addAction(reset);
    input = new QAction(QIcon(":/source/source/input.png"), tr("input with keyboard"), toolBar);
    input->setStatusTip("input with keyboard");
    input->setToolTip(tr("input with keyboard"));
    connect(input, &QAction::triggered, this, &Controlpanel::input_fromDialog);
    toolBar->addAction(input);
    save = new QAction(QIcon(":/source/source/save.png"), tr("save canvas(Ctrl + S for short)"), toolBar);
    save->setStatusTip(tr("save canvas(Ctrl + S for short)"));
    save->setToolTip(tr("save canvas(Ctrl + S for short)"));
    save->setShortcut(QKeySequence::Save);
    connect(save, &QAction::triggered, this, &Controlpanel::save_Canvas);
    toolBar->addAction(save);
    isCuttingLine = false;
    isDrawingLine = false;
    isDrawingCircle = false;
    isDrawingRectangle = false;
    isDrawingOval = false;
    isDrawingPolygon = false;
    isDrawingCurve = false;
    isReleased = false;
    isDraggingShape = false;
    allowDragging = false;
    connect(drawpad, &Drawpad::postionChange, this, &Controlpanel::display_Information);
    connect(drawpad, &Drawpad::quitFunctions, this, &Controlpanel::quit_AllFunctions);
}

Controlpanel::~Controlpanel()
{
    delete ui;
}

bool Controlpanel::save_Canvas() {
    return drawpad->save_Canvas();
}

void Controlpanel::reset_Canvas() {
    int w = QInputDialog::getInt(nullptr, "width",
                                "w:", 100, 100, 1000, 1, nullptr);
    int h = QInputDialog::getInt(nullptr, "height",
                                "h:", 100, 100, 800, 1, nullptr);
    drawpad->reset_Canvas(w, h);
}

void Controlpanel::clean_Canvas() {
    drawpad->clean_Canvas();
}

void Controlpanel::set_inkColor() {
    drawpad->set_inkColor();
}

void Controlpanel::set_penWidth() {
    drawpad->set_penWidth();
}

void Controlpanel::draw_Line() {
    int method = 0;
    if(!isDrawingLine) {
        quit_AllFunctions();
        QStringList algorithmList;
        algorithmList << "DDA" << "Bresenham";
        QString algorithm = QInputDialog::getItem(nullptr, "Algorithm",
                                                  "Please select an algorithm:", algorithmList, 1, true, nullptr);

        if(algorithm == "Bresenham")
            method = 1;
        drawpad->Line_switch(true, method);
        isDrawingLine = true;
    }
    else {
        drawpad->Line_switch(false, method);
        //when closing this function, method is useless
        isDrawingLine = false;
    }
}

void Controlpanel::draw_Circle() {
    if(!isDrawingCircle) {
        quit_AllFunctions();
        drawpad->Circle_switch(true);
        isDrawingCircle = true;
    }
    else {
        drawpad->Circle_switch(false);
        isDrawingCircle = false;
    }
}

void Controlpanel::draw_Rectangle() {
    int method = 0;
    if(!isDrawingRectangle) {
        quit_AllFunctions();
        QStringList algorithmList;
        algorithmList << "DDA" << "Bresenham";
        QString algorithm = QInputDialog::getItem(nullptr, "Algorithm",
                                                  "Please select an algorithm:", algorithmList, 1, true, nullptr);
        if(algorithm == "Bresenham")
            method = 1;
        drawpad->Rectangle_switch(true, method);
        isDrawingRectangle = true;
    }
    else {
        drawpad->Rectangle_switch(false, method);
        isDrawingRectangle = false;
    }
}

void Controlpanel::draw_Oval() {
    if(!isDrawingOval) {
        quit_AllFunctions();
        drawpad->Oval_switch(true);
        isDrawingOval = true;
    }
    else {
        drawpad->Oval_switch(false);
        isDrawingOval = false;
    }
}

void Controlpanel::draw_Polygon() {
    int method = 0;
    if(!isDrawingPolygon) {
        quit_AllFunctions();
        QStringList algorithmList;
        algorithmList << "DDA" << "Bresenham";
        QString algorithm = QInputDialog::getItem(nullptr, "Algorithm",
                                                  "Please select an algorithm:", algorithmList, 1, true, nullptr);
        if(algorithm == "Bresenham")
            method = 1;
        drawpad->Polygon_switch(true, method);
        isDrawingPolygon = true;
    }
    else {
        drawpad->Polygon_switch(false, method);
        isDrawingPolygon = false;
    }
}


void Controlpanel::draw_Curve() {
    int method = 0;
    if(!isDrawingCurve) {
        quit_AllFunctions();
        QStringList algorithmList;
        algorithmList << "Bezier" << "B-spline";
        QString algorithm = QInputDialog::getItem(nullptr, "Algorithm",
                                                  "Please select an algorithm:", algorithmList, 1, true, nullptr);
        if(algorithm == "B-spline")
            method = 1;
        drawpad->Curve_switch(true, method);
        isDrawingCurve = true;
    }
    else {
        drawpad->Curve_switch(false, method);
        isDrawingCurve = false;
    }
}

void Controlpanel::allow_dragging() {
    if(!allowDragging) {
        quit_AllFunctions();
        drawpad->drag_switch(true);
        allowDragging = true;
    }
    else {
        drawpad->drag_switch(false);
        allowDragging = false;
    }
}

void Controlpanel::input_fromDialog() {
    drawpad->input_fromDialog();
}

void Controlpanel::display_Information(QPoint pos, QPoint point, int shapeIdx, int type) {
    QString shape;
    enum {SHAPE, LINE, CIRCLE, RECTANGLE, OVAL, POLYGON, CURVE};
    switch (type) {
        case SHAPE:
            shape = "Shape(Null)";
            break;
        case LINE:
            shape = "Line";
            break;
        case CIRCLE:
            shape = "Circle";
            break;
        case RECTANGLE:
            shape = "Rectangle";
            break;
        case OVAL:
            shape = "Oval";
            break;
        case POLYGON:
            shape = "Polygon";
            break;
        case CURVE:
            shape = "Curve";
            break;
        default:
            shape = "Shape(Null)";
    }
    QString message = "current cursor pos: (";
    message = message + QString::number(pos.x()) + ", " + QString::number(pos.y()) + ");"
            + " current shape id: " + QString::number(shapeIdx) + "; current shape type: " + shape
            + "; transform center: (" + QString::number(point.x()) + ", " + QString::number(point.y()) + ")";
    mainWindow->statusBar()->showMessage(message);
}

void Controlpanel::quit_AllFunctions() {
    if(isDrawingLine) drawLine->trigger();
    if(isDrawingCircle) drawCircle->trigger();
    if(isDrawingOval) drawOval->trigger();
    if(isDrawingRectangle) drawRectangle->trigger();
    if(isDrawingPolygon) drawPolygon->trigger();
    if(isDrawingCurve) drawCurve->trigger();
    if(allowDragging) drag->trigger();
}
