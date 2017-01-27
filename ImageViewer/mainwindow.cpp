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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gv = ui->graphicsView;
    scene = new ImageScene(this);
    scene->angleSlider = ui->angleHSlider;
    scene->setMode(ImageScene::ZoomIn);
    
    connect(ui->angleHSlider , SIGNAL(valueChanged(int)) , ui->angleSpinBox , SLOT(setValue(int)));
    connect(ui->angleSpinBox , SIGNAL(valueChanged(int)) , ui->angleHSlider , SLOT(setValue(int)));
    
    /* for debugging */
    display(":/Images_list/Images/default");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    save_changes();
}

/* open */
void MainWindow::on_openBtn_clicked()
{
    save_changes();
    QString imagePath = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                tr("JPEG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp)")
                );
    if (imagePath.size() == 0)
        return;
    reset();
    display(imagePath);
}

/*save*/
void MainWindow::on_saveBtn_clicked()
{
    QString imagePath = QFileDialog::getSaveFileName(
                    this,
                    tr("Save Image"),
                    "",
                    tr("PNG (*.png);;JPEG (*.jpeg)"));
    if (imagePath.size() == 0)
        return;
    /* Save Image */
    QFile file(imagePath);
    file.open(QIODevice::WriteOnly);
    image->currentQImage()->save(&file);
    file.close();

    display(imagePath);
}

/* zoom out */
void MainWindow::on_zoomOutBtn_clicked()
{
    gv->scale(0.5, 0.5);
}

/* crop */
void MainWindow::on_cropBtn_clicked()
{
    scene->setMode(ImageScene::Crop);
}

/* slider */
void MainWindow::on_angleHSlider_valueChanged(int value)
{
    rotate(value);
}

/* angleSpinBox */
void MainWindow::on_angleSpinBox_valueChanged(int value)
{
    rotate(value);
}

/* reset */
void MainWindow::on_resetBtn_clicked()
{
    if(image->changed()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
                    this, "Warning", "Changes will be lost, are you sure you want to reset?",
                                        QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes)
            display(image->imagePath());
    }
}

void MainWindow::rotate(int value){
    image->rotate(value);
    scene->setImage(image);
}

void MainWindow::display(QString path){
    if (!image)
      delete image;
    image = new Image(path);
    this->setWindowTitle("Image Viewer::" + path);
    reset();
    gv->setScene(scene);
    scene->setMode(ImageScene::ZoomIn);
}

void MainWindow::reset() {
    scene->setImage(image);
    scene->setMode(ImageScene::ZoomIn);
    ui->angleHSlider->setValue(0);
    gv->resetMatrix();
}

void MainWindow::save_changes() {
    if(image->changed()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
                    this, "Save Changes", "Do you want to save changes?",
                                        QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes)
            on_saveBtn_clicked();
    }
}
