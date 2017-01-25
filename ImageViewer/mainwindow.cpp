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
    connect(ui->horizontalSlider , SIGNAL(valueChanged(int)) , ui->spinBox , SLOT(setValue(int)));
    connect(ui->spinBox , SIGNAL(valueChanged(int)) , ui->horizontalSlider , SLOT(setValue(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*open*/
void MainWindow::on_pushButton_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                tr("JPEG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp)" )
                );
    display(imagePath);
    ui->horizontalSlider->setValue(0);

}

/*save*/
void MainWindow::on_pushButton_2_clicked()
{
    QString imagePath = QFileDialog::getSaveFileName(
                    this,
                    tr("Save File"),
                    "",
                    tr("JPEG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp)" )
                    );

    *imageObject = image.toImage();

    /*Save Image*/
    QFile file( imagePath);
    file.open( QIODevice::WriteOnly );
    imageObject->save(&file,"PNG");
    file.close();

}

/*zoom*/
void MainWindow::on_pushButton_3_clicked()
{

}

/*crop*/
void MainWindow::on_pushButton_4_clicked()
{

}

/*slider*/
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    rotate(value);
}

/*spinBOX*/
void MainWindow::on_spinBox_valueChanged(int value)
{
    rotate(value);
}

/*reset*/
void MainWindow::on_pushButton_5_clicked()
{
    display(":/Images_list/Images/default");
    rotate(0);
    ui->horizontalSlider->setValue(0);

}
