#include "drawpad.h"
#include "ui_drawpad.h"
#include <QColorDialog>
#include <QInputDialog>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

Drawpad::Drawpad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Drawpad)
{
    ui->setupUi(this);
    setFixedSize(1000, 800);
    pix = QPixmap(500, 500);
    pix.fill(Qt::white);
    inkColor.setRgb(0, 0, 0);
    penWidth = 1;
    drawMethod_line = 0;
    drawMethod_curve = 0;
    transformPoint.setX(0);
    transformPoint.setY(0);
    pressPoint.setX(0);
    pressPoint.setY(0);
    releasePoint.setX(0);
    releasePoint.setY(0);
    currentShapeIdx = -1;
    cutAlgorithm = "";
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
    isResizing = false;
    delta_x = 0;
    delta_y = 0;

    init_Pen();
}

Drawpad::~Drawpad()
{
    delete ui;
}

void Drawpad::init_Pen() {
    pen.setColor(inkColor);
    pen.setWidth(penWidth);
}

bool Drawpad::save_Canvas() {
    QFileDialog qfd(this);
    qfd.setDefaultSuffix("bmp");
    QString fPath = qfd.getSaveFileName(this, tr("Save File"),
                    "MyCanvas.bmp", tr("BMP(*.bmp)"));

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

void Drawpad::clean_Canvas() {
    pix.fill(Qt::white);
    shapePen.clear();
    shapeQueue.clear();
}

void Drawpad::reset_Canvas(int width, int height) {
    pix = QPixmap(width, height);
    pix.fill(Qt::white);
    pix.scaled(this->width(),this->height());
    redraw_Shapes();
    update();
}

void Drawpad::set_inkColor() {
    QColorDialog askForColor;
    inkColor = askForColor.getColor();
    init_Pen();
}

void Drawpad::set_penWidth() {
    penWidth = QInputDialog::getInt(nullptr, "width",
                                "pen width:", penWidth, 1, 10, 1, nullptr);
    init_Pen();
}

void Drawpad::redraw_Shapes() {
    pix.fill(Qt::white);
    QPen tmp;
    for(int i = 0; i < shapeQueue.size(); i++) {
        tmp = shapePen[i];
        shapeQueue[i]->draw(tmp, pix);
    }
    if(transformPoint.x() != 0 && transformPoint.y() != 0) {
        Circle *redDot = new Circle(transformPoint, 2);
        tmp.setColor(QColor(255, 0, 0));
        tmp.setStyle(Qt::SolidLine);
        tmp.setWidth(1);
        redDot->draw(tmp, pix);
    }
    update();
}

void Drawpad::input_fromDialog() {
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
        shapePen.push_back(pen);
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
        shapePen.push_back(pen);
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
        shapePen.push_back(pen);
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
        shapePen.push_back(pen);
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
        shapePen.push_back(pen);
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
        shapePen.push_back(pen);
        newCurve->draw(pen, pix);
    }
}

void Drawpad::Line_switch(bool flag, int method) {
    isDrawingLine = flag;
    drawMethod_line = method;
}

void Drawpad::draw_Line(QPoint startPoint, QPoint endPoint) {
    Line *newLine = new Line(startPoint, endPoint);
    newLine->setDrawMethod(drawMethod_line);
    shapeQueue.push_back(newLine);
    shapePen.push_back(pen);
    newLine->draw(pen, pix);
}

void Drawpad::Circle_switch(bool flag) {
    isDrawingCircle = flag;
}

void Drawpad::draw_Circle(QPoint point1, QPoint point2) {
    Circle *newCircle = new Circle(point1, point2);
    shapeQueue.push_back(newCircle);
    shapePen.push_back(pen);
    newCircle->draw(pen, pix);
}

void Drawpad::Rectangle_switch(bool flag, int method) {
    isDrawingRectangle = flag;
    drawMethod_line = method;
}

void Drawpad::draw_Rectangle(QPoint point1, QPoint point2) {
    Rectangle *newRectange = new Rectangle(point1, point2);
    shapeQueue.push_back(newRectange);
    shapePen.push_back(pen);
    newRectange->draw(pen, pix);
}

void Drawpad::Oval_switch(bool flag) {
    isDrawingOval = flag;
}

void Drawpad::draw_Oval(QPoint point1, QPoint point2) {
    Oval *newOval = new Oval(point1, point2);
    shapeQueue.push_back(newOval);
    shapePen.push_back(pen);
    newOval->draw(pen, pix);
}

void Drawpad::Polygon_switch(bool flag, int method) {
    isDrawingPolygon = flag;
    drawMethod_line = method;
    if(flag == false) {
        pressPoints.clear();
        redraw_Shapes();
    }
}

void Drawpad::draw_Polygon(QVector<QPoint> points) {
    Polygon *newPolygon = new Polygon(points);
    shapeQueue.push_back(newPolygon);
    shapePen.push_back(pen);
    newPolygon->draw(pen, pix);
}

void Drawpad::Curve_switch(bool flag, int method) {
    isDrawingCurve = flag;
    drawMethod_curve = method;
    if(flag == false) {
        pressPoints.clear();
        redraw_Shapes();
    }
}

void Drawpad::draw_Curve(QVector<QPoint> points) {
    Curve *newCurve = new Curve(points);
    newCurve->setDrawMethod(drawMethod_curve);
    shapeQueue.push_back(newCurve);
    shapePen.push_back(pen);
    newCurve->draw(pen, pix);
}

void Drawpad::drag_switch(bool flag) {
    allowDragging = flag;
}

bool Drawpad::choose_Shape(QPoint cursor) {
    if(shapeQueue.size() == 0)
        return false;
    for(int i = 0; i < shapeQueue.size(); i++) {
        if(shapeQueue[i]->in_DraggingArea(cursor)) {
            currentShapeIdx = i;
            return true;
        }
    }
    return false;
}

void Drawpad::show_auxiliaryPoint(QPoint point) {
    Circle *greenDot = new Circle(point, 2);
    QPen tmp;
    tmp.setColor(QColor(0, 255, 0));
    tmp.setStyle(Qt::SolidLine);
    tmp.setWidth(1);
    greenDot->draw(tmp, pix);
}

void Drawpad::show_auxiliaryRec(Shape *s) {
    Rectangle *blueRec = new Rectangle(s->getLT(), s->getRB());
    QPen tmp;
    tmp.setColor(QColor(0, 0, 255));
    tmp.setStyle(Qt::SolidLine);
    tmp.setWidth(1);
    blueRec->draw(tmp, pix);
}

void Drawpad::finish_currentDrawing() {
    if(isDrawingPolygon)
        draw_Polygon(pressPoints);
    else if(isDrawingCurve)
        draw_Curve(pressPoints);
    pressPoints.clear();
    emit quitFunctions();
    redraw_Shapes();
}

void Drawpad::set_transformPoint() {
    transformPoint = pressPoint;
    redraw_Shapes();
}

void Drawpad::set_transformPointInput() {
    int x = QInputDialog::getInt(nullptr, "set x", "x:", 0, 1, pix.width(), 1, nullptr);
    int y = QInputDialog::getInt(nullptr, "set y", "y:", 0, 1, pix.height(), 1, nullptr);
    transformPoint.setX(x);
    transformPoint.setY(y);
    redraw_Shapes();
    show_auxiliaryRec(shapeQueue[currentShapeIdx]);
}

void Drawpad::rotate_currentShape(int degrees) {
    int x = transformPoint.x();
    int y = transformPoint.y();
    shapeQueue[currentShapeIdx]->rotate(x, y, degrees);
    redraw_Shapes();
    show_auxiliaryRec(shapeQueue[currentShapeIdx]);
}

void Drawpad::rotate_currentShape90() {
    rotate_currentShape(90);
}

void Drawpad::rotate_currentShape180() {
    rotate_currentShape(180);
}

void Drawpad::rotate_currentShapeInput() {
    int degrees = QInputDialog::getInt(nullptr, "degrees", "degree:", 0, 1, 360, 1, nullptr);
    rotate_currentShape(degrees);
}

void Drawpad::scale_currentShape() {
    float ratio = (float)QInputDialog::getDouble(nullptr, "ratio", "ratio:", 1, 0.01, 100, 2, nullptr);
    int x = transformPoint.x();
    int y = transformPoint.y();
    shapeQueue[currentShapeIdx]->scale(x, y, ratio);
    redraw_Shapes();
}

void Drawpad::delete_currentShape() {
    shapeQueue.remove(currentShapeIdx);
    shapePen.remove(currentShapeIdx);
    currentShapeIdx = -1;
    redraw_Shapes();
}

void Drawpad::cut_Line() {
    isCuttingLine = true;
    QStringList algorithmList;
    algorithmList << "Cohen-Sutherland" << "Liang-Barsky";
    cutAlgorithm = QInputDialog::getItem(nullptr, "Algorithm",
                    "Please select an algorithm:", algorithmList, 1, true, nullptr);
    setCursor(Qt::CrossCursor);
    emit quitFunctions();
}

void Drawpad::paintEvent(QPaintEvent *event) {
    init_Pen();
    if(isDrawingLine && isReleased && !isDraggingShape) {
        draw_Line(pressPoint, releasePoint);
        isReleased = false;
        redraw_Shapes();
    }

    if(isDrawingCircle && isReleased && !isDraggingShape) {
        draw_Circle(pressPoint, releasePoint);
        isReleased = false;
        redraw_Shapes();
    }
    if(isDrawingRectangle && isReleased && !isDraggingShape) {
        draw_Rectangle(pressPoint, releasePoint);
        isReleased = false;
        redraw_Shapes();
    }

    if(isDrawingOval && isReleased && !isDraggingShape) {
        draw_Oval(pressPoint, releasePoint);
        isReleased = false;
        redraw_Shapes();
    }
    if(isCuttingLine && isReleased && !isDraggingShape && currentShapeIdx != -1) {
        shapeQueue[currentShapeIdx]->clip(pressPoint, releasePoint, cutAlgorithm.toStdString());
        redraw_Shapes();
    }
    if(isReleased) {
        if(isDraggingShape) {
            isDraggingShape = false;
            setCursor(Qt::ArrowCursor);
            redraw_Shapes();
            show_auxiliaryRec(shapeQueue[currentShapeIdx]);
        }
        if(isCuttingLine) {
            isCuttingLine = false;
            setCursor(Qt::ArrowCursor);
            redraw_Shapes();
            show_auxiliaryRec(shapeQueue[currentShapeIdx]);
        }
        if(isResizing) {
            isResizing = false;
            setCursor(Qt::ArrowCursor);
            redraw_Shapes();
        }
        isReleased = false;
    }

    QPainter Painter(this);
    Painter.drawPixmap(0, 0, pix.width(), pix.height(), pix);
    update();
}

void Drawpad::mousePressEvent(QMouseEvent *event) {
    if(event->button()==Qt::LeftButton) {
        isReleased = false;
        pressPoint = event->pos();
        currentPos = event->pos();
        if(isDrawingLine || isDrawingOval ||
           isDrawingCircle || isDrawingRectangle) {
            redraw_Shapes();
            show_auxiliaryPoint(pressPoint);
        }
        else if(!isDraggingShape && (isDrawingPolygon || isDrawingCurve) && !isCuttingLine) {
            redraw_Shapes();
            pressPoints.push_back(pressPoint);
            for(int i = 0; i < pressPoints.size(); i++)
                show_auxiliaryPoint(pressPoints[i]);
        }
        else if(!isDraggingShape && !isDrawingPolygon && !isDrawingCurve && !isCuttingLine) {
            if(abs(pressPoint.x() - pix.width()) <= 3 && abs(pressPoint.y() - pix.height()) <= 3) {
                isResizing = true;
                setCursor(Qt::SizeFDiagCursor);
            }
            if(choose_Shape(pressPoint)) {
                if(allowDragging) {
                    isDraggingShape = true;
                    setCursor(Qt::SizeAllCursor);
                }
                redraw_Shapes();
                show_auxiliaryRec(shapeQueue[currentShapeIdx]);
            }
            else
                currentShapeIdx = -1;
        }
    }
}

void Drawpad::mouseMoveEvent(QMouseEvent* event) {
    QPoint temp = currentPos;
    currentPos = event->pos();
    int type = 0;
    if(currentShapeIdx != -1 && !isCuttingLine)
        type = shapeQueue[currentShapeIdx]->getType();
    emit postionChange(currentPos, transformPoint, currentShapeIdx, type);
    redraw_Shapes();
    if((isDrawingLine || isDrawingOval ||
        isDrawingCircle || isDrawingRectangle) &&
        !isDraggingShape && !isCuttingLine && !isResizing) {
        show_auxiliaryPoint(pressPoint);
        show_auxiliaryPoint(currentPos);
        if(isDrawingLine) {
            Line newLine(pressPoint, currentPos);
            newLine.draw(pen, pix);
        }
        if(isDrawingOval) {
            Oval newOval(pressPoint, currentPos);
            newOval.draw(pen, pix);
        }
        if(isDrawingCircle) {
            Circle newCircle(pressPoint, currentPos);
            newCircle.draw(pen, pix);
        }
        if(isDrawingRectangle) {
            Rectangle newRec(pressPoint, currentPos);
            newRec.draw(pen, pix);
        }
    }

    else if((isDrawingPolygon || isDrawingCurve) &&
        !isDraggingShape && !isCuttingLine && !isResizing) {
        for(int i = 0; i < pressPoints.size(); i++)
            show_auxiliaryPoint(pressPoints[i]);
    }

    else if(isDraggingShape && !isCuttingLine && !isResizing) {
        shapeQueue[currentShapeIdx]->translate(currentPos.x() - temp.x(), currentPos.y() - temp.y());
        redraw_Shapes();
        show_auxiliaryRec(shapeQueue[currentShapeIdx]);

    }

    else if (!isCuttingLine && currentShapeIdx != -1 && !isResizing) {
        show_auxiliaryRec(shapeQueue[currentShapeIdx]);
    }
    else if(isResizing) {
        int x = currentPos.x();
        int y = currentPos.y();
        if(x > 1000)
            x = 1000;
        if(y > 800)
            y = 800;
        if(x < 100)
            x = 100;
        if(y < 100)
            y = 100;
        reset_Canvas(x, y);
    }
}

void Drawpad::mouseReleaseEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        releasePoint = event->pos();
        isReleased = true;
        update();
        if(isResizing) {
            int x = currentPos.x();
            int y = currentPos.y();
            if(x > 1000)
                x = 1000;
            if(y > 800)
                y = 800;
            if(x < 100)
                x = 100;
            if(y < 100)
                y = 100;
            reset_Canvas(x, y);
        }
    }
}

void Drawpad::contextMenuEvent(QContextMenuEvent* event) {
    QMenu contex;
    pressPoint = event->pos();

    if(!isDraggingShape && (isDrawingPolygon || isDrawingCurve) && !isCuttingLine) {
        QAction *finish_drawing;
        finish_drawing = new QAction(tr("finish drawing polygon/curve"), this);
        finish_drawing->setStatusTip("finish drawing polygon/curve");
        connect(finish_drawing, &QAction::triggered, this, &Drawpad::finish_currentDrawing);
        contex.addAction(finish_drawing);
    }
    else {
        if(currentShapeIdx != -1) {
            show_auxiliaryRec(shapeQueue[currentShapeIdx]);
            QAction *rotate_90_degree, *rotate_180_degree, *rotate_input_degree,
                    *scale, *cutLine, *delete_current;

            rotate_90_degree = new QAction(tr("rotate 90° clockwisely"), this);
            rotate_90_degree->setStatusTip("rotate 90° clockwisely");
            connect(rotate_90_degree, &QAction::triggered, this, &Drawpad::rotate_currentShape90);

            rotate_180_degree = new QAction(tr("rotate 180° clockwisely"), this);
            rotate_180_degree->setStatusTip("rotate 180° clockwisely");
            connect(rotate_180_degree, &QAction::triggered, this, &Drawpad::rotate_currentShape180);

            rotate_input_degree = new QAction(tr("rotate input degrees clockwisely"), this);
            rotate_input_degree->setStatusTip("rotate input degrees clockwisely");
            connect(rotate_input_degree, &QAction::triggered, this, &Drawpad::rotate_currentShapeInput);

            scale = new QAction(tr("scale"), this);
            scale->setStatusTip("scale");
            connect(scale, &QAction::triggered, this, &Drawpad::scale_currentShape);

            cutLine = new QAction(tr("cut a line"), this);
            cutLine->setStatusTip("cut a line");
            connect(cutLine, &QAction::triggered, this, &Drawpad::cut_Line);

            delete_current = new QAction(tr("delete chosen shape"), this);
            delete_current->setStatusTip("delete chosen shape");
            connect(delete_current, &QAction::triggered, this, &Drawpad::delete_currentShape);

            contex.addAction(rotate_90_degree);
            contex.addAction(rotate_180_degree);
            contex.addAction(rotate_input_degree);
            contex.addAction(scale);
            if(shapeQueue[currentShapeIdx]->getType() == 1)
                contex.addAction(cutLine);
            contex.addAction(delete_current);
        }
        else {
            QAction *set_transform_point, *set_transform_point_input;

            set_transform_point = new QAction(tr("set transform center"), this);
            set_transform_point->setStatusTip("set transform center");
            connect(set_transform_point, &QAction::triggered, this, &Drawpad::set_transformPoint);
            set_transform_point_input = new QAction(tr("set transform center from keyboard"), this);
            set_transform_point_input->setStatusTip("set transform center from keyboard");
            connect(set_transform_point_input, &QAction::triggered, this, &Drawpad::set_transformPointInput);

            contex.addAction(set_transform_point);
            contex.addAction(set_transform_point_input);
        }
    }
    QPoint pos = QCursor::pos();
    pos.setX(pos.x() + 1);
    pos.setY(pos.y() + 1);
    contex.exec(pos);
}

void Drawpad::wheelEvent(QWheelEvent* event) {
    if(currentShapeIdx != -1) {
        float ratio = 1;
        if(event->delta() > 0)
            ratio = 1.10;
        if(event->delta() < 0)
            ratio = 0.90;
        int x = shapeQueue[currentShapeIdx]->getCenter().x();
        int y = shapeQueue[currentShapeIdx]->getCenter().y();
        shapeQueue[currentShapeIdx]->scale(x, y, ratio);
        redraw_Shapes();
        show_auxiliaryRec(shapeQueue[currentShapeIdx]);
    }
}
