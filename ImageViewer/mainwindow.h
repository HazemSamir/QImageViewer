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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

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
