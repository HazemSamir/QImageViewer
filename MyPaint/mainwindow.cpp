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

QLabel *label;
QScrollArea *scrollArea;
QLineEdit *lineEdit;
int width;
int height;


/*show img in the label_img place*/
void MainWindow::show_img(QString str){
    QPixmap img(str);
    scrollArea->setWidget(label);
    scrollArea->setWidgetResizable(true);
    label->setPixmap(img);
    scrollArea->show();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scrollArea = ui->scrollArea;
    label = new QLabel;
    lineEdit = ui->lineEdit;

    /*load the default img (white img)*/
    show_img(":/img_resources/Imgs/default.jpg");

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*open img file*/
void MainWindow::on_pushButton_clicked()
{
    /*Show file chose dialog*/
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.jpg *.bmp)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open Image"), "/home/mohamed", tr("Image Files (*.png *.jpg *.bmp)"));

    /*Load Image*/
    show_img(fileName);
}

void MainWindow::on_pushButton_2_clicked()
{
    /*Show file chose dialog*/
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.jpg *.bmp)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save Image"),"/home/mohamed",tr("Image Files (*.png *.jpg *.bmp)"));

    /*Save Image*/
    QFile file( fileName );
    file.open( QIODevice::WriteOnly );
    label->pixmap()->save(&file,"PNG");
    file.close();

}

/*rotate with certain angle*/
void MainWindow::on_pushButton_3_clicked()
{

    int angle = lineEdit->text().toInt();

    int w = scrollArea->width();
    int h = scrollArea->height();

    QPixmap pixmap(*label->pixmap());

    QMatrix rm;
    rm.rotate(angle);
    pixmap = pixmap.transformed(rm);
    pixmap.scaled(w,h,Qt::KeepAspectRatio);
    label->setPixmap(pixmap);
    scrollArea->baseSize();

}
