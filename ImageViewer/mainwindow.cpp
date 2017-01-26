#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QScrollArea>
#include <QLineEdit>
#include <QTransform>
#include <QtGui>

QGraphicsView *gv;
int rotation_angle = 0;

void MainWindow::rotate(int value){
    int delta = value - rotation_angle;
    rotation_angle = value;
    gv->rotate(delta);
}

void MainWindow::display(QString str){
    imageObject = new QImage();
    imageObject->load(str);
    image = QPixmap::fromImage(*imageObject);

    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    gv->setScene(scene);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gv = ui->graphicsView;
    display(":/Images_list/Images/default");
    connect(ui->angleHSlider , SIGNAL(valueChanged(int)) , ui->angleSpinBox , SLOT(setValue(int)));
    connect(ui->angleSpinBox , SIGNAL(valueChanged(int)) , ui->angleHSlider , SLOT(setValue(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*open*/
void MainWindow::on_openBtn_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                tr("JPEG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp)" )
                );
    display(imagePath);
    ui->angleHSlider->setValue(0);
}

/*save*/
void MainWindow::on_saveBtn_clicked()
{
    QString imagePath = QFileDialog::getSaveFileName(
                    this,
                    tr("Save Image"),
                    "",
                    tr("JPEG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp)" )
                    );

    *imageObject = image.toImage();

    /*Save Image*/
    QMatrix rm;
    int angle = ui->angleHSlider->value();
    rm.rotate(angle);
    QImage newImage = imageObject->transformed(rm, Qt::SmoothTransformation);
    // pixmap.scaled(w, h, Qt::KeepAspectRatio);
    QFile file(imagePath);
    file.open(QIODevice::WriteOnly);
    newImage.save(&file, "PNG");
    file.close();
}

/*zoom*/
void MainWindow::on_zoomBtn_clicked()
{

}

/*crop*/
void MainWindow::on_cropBtn_clicked()
{

}

/*slider*/
void MainWindow::on_angleHSlider_valueChanged(int value)
{
    rotate(value);
}

/*angleSpinBox*/
void MainWindow::on_angleSpinBox_valueChanged(int value)
{
    rotate(value);
}

/*reset*/
void MainWindow::on_resetBtn_clicked()
{
    display(":/Images_list/Images/default");
    rotate(0);
    ui->angleHSlider->setValue(0);
}
