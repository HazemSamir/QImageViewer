#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsScene>
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

    void on_zoomBtn_clicked();

    void on_cropBtn_clicked();

    void on_resetBtn_clicked();

    void on_angleHSlider_valueChanged(int value);

    void on_angleSpinBox_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QGraphicsPixmapItem *item;
    QPixmap image;
    QGraphicsScene *scene;
    QImage  *imageObject;
    void display(QString str);
    void rotate(int value);
};

#endif // MAINWINDOW_H
