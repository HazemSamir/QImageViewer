#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "image.h"
#include "imagescene.h"

#include <QMainWindow>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    enum ActionType {
        Zoom,
        Rotate,
        Crop
    };

    struct Action {
        ActionType type;
        int angle = 0;
        int zoomFactor = 100;
        Image* image = 0;
    };

    explicit MainWindow(QWidget* parent = 0);
    void display(QString str);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event);
    void keyPressEvent(QKeyEvent* event);

private slots:

    void on_angleHSlider_valueChanged(int);
    void on_angleSpinBox_valueChanged(int);
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void on_actionReset_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionCrop_triggered(bool checked);
    void on_actionHandTool_triggered(bool checked);
    void on_actionZoomInArea_triggered(bool checked);
    void on_actionRotate_triggered(bool checked);
    void on_actionZoomFactor_triggered(bool checked);
    void on_toolBar_actionTriggered(QAction*);
    void on_menuBar_actionTriggered(QAction*);

    void on_scene_modeChanged(int);
    void absoluteZoom(double);
    void absoluteZoom(int);

    void doAction(Action*);
    void saveCropAction();
    void saveRotateAction(int);
    void saveZoomAction(int);

private:
    static const int MAX_UNDO_STACK_SIZE = 1024;

    Ui::MainWindow* ui = 0;
    ImageScene* scene = 0;
    QGraphicsView* gv = 0;
    Image* image = 0;
    int gv_lazy_rotation = 0;

    bool saveActions = true;
    QStack<Action *> undoStack, redoStack;

    int zoomFactorPercentage();
    double zoomFactor();
    void save_changes();
    void saveActionHelper();
    void lazy_rotate(int value);
    void propagate_lazy_rotate();
    void reset();
};

#endif // MAINWINDOW_H
