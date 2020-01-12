#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QDialog>
#include <QCloseEvent>
#include "drawpad.h"

namespace Ui {
class Controlpanel;
}

class Controlpanel : public QDialog
{
    Q_OBJECT

public:
    explicit Controlpanel(QWidget *parent = nullptr);
    ~Controlpanel();

private:
    bool save_Canvas();
    void reset_Canvas();
    void clean_Canvas();
    void draw_Line();
    void draw_Circle();
    void draw_Rectangle();
    void draw_Oval();
    void draw_Polygon();
    void draw_Curve();
    void allow_dragging();
    void set_inkColor();
    void set_penWidth();
    void input_fromDialog();
    void display_Information(QPoint pos, QPoint point, int shapeIdx, int type);
    void quit_AllFunctions();

private:
    Ui::Controlpanel *ui;
    QMainWindow *mainWindow;
    QToolBar *toolBar;
    Drawpad *drawpad;
    QAction *drawLine, *drawCurve, *drawCircle, *drawOval, *drawRectangle, *drawPolygon,
            *drag, *setColor, *setPen, *resize, *reset, *input, *save;
    bool isCuttingLine;
    bool isDrawingLine;
    bool isDrawingCircle;
    bool isDrawingRectangle;
    bool isDrawingOval;
    bool isDrawingPolygon;
    bool isDrawingCurve;
    bool isReleased;
    bool allowDragging;
    bool isDraggingShape;
};

#endif // CONTROLPANEL_H
