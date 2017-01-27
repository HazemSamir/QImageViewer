#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imagescene.h"
#include "image.h"

#include <QMainWindow>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_openBtn_clicked();

    void on_saveBtn_clicked();

    void on_zoomOutBtn_clicked();

    void on_cropBtn_clicked();

    void on_resetBtn_clicked();

    void on_angleHSlider_valueChanged(int value);

    void on_angleSpinBox_valueChanged(int value);

private:
    Ui::MainWindow *ui = 0;
    ImageScene *scene = 0;
    QGraphicsView *gv = 0;
    
    Image *image = 0;
    
    void display(QString str);
    void rotate(int value);
    void reset();
};

#endif // MAINWINDOW_H
