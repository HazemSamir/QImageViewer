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
    scene->setMode(ImageScene::NoMode);
    
    connect(ui->angleHSlider , SIGNAL(valueChanged(int)) , ui->angleSpinBox , SLOT(setValue(int)));
    connect(ui->angleSpinBox , SIGNAL(valueChanged(int)) , ui->angleHSlider , SLOT(setValue(int)));
    
    resize(QGuiApplication::primaryScreen()->availableSize() * 4 / 5);

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
void MainWindow::on_actionOpen_triggered()
{
    scene->setMode(ImageScene::NoMode);
    save_changes();
    QString imagePath = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                tr("Image (*.jpg *.jpeg *.png *.bmp)")
                );
    if (imagePath.size() == 0)
        return;
    reset();
    display(imagePath);
}

/* save */
void MainWindow::on_actionSave_triggered()
{
    scene->setMode(ImageScene::NoMode);
    QString imagePath = QFileDialog::getSaveFileName(
                    this,
                    tr("Save Image"),
                    "",
                    tr("PNG (*.png);;JPEG (*.jpeg);;BMP (*.bmp)"));
    if (imagePath.size() == 0)
        return;
    /* Save Image */
    QFile file(imagePath);
    file.open(QIODevice::WriteOnly);
    image->currentQImage()->save(&file);
    file.close();

    display(imagePath);
}

/* zoom in */
void MainWindow::on_actionZoomIn_triggered()
{
    scene->setMode(ImageScene::ZoomIn);
}

/* zoom out */
void MainWindow::on_actionZoomOut_triggered()
{
    scene->setMode(ImageScene::NoMode);
    gv->scale(scene->zoomOutFactor, scene->zoomOutFactor);
}

/* crop */
void MainWindow::on_actionCrop_triggered()
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
void MainWindow::on_actionReset_triggered()
{
    if(image->changed()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
                    this, "Warning", "Changes will be lost, are you sure you want to reset?",
                                        QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes)
            display(image->imagePath());
    } else {
        reset();
    }
}

void MainWindow::rotate(int value)
{
    image->rotate(value);
    scene->setImage(image);
}

void MainWindow::display(QString path)
{
    if (!image)
      delete image;
    image = new Image(path);
    this->setWindowTitle("Image Viewer::" + path);
    reset();
    gv->setScene(scene);
}

void MainWindow::reset()
{
    scene->setImage(image);
    scene->setMode(ImageScene::NoMode);
    ui->angleHSlider->setValue(0);
    gv->resetMatrix();
}

void MainWindow::save_changes()
{
    if(image->changed()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
                    this, "Save Changes", "Do you want to save changes?",
                                        QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes)
            on_actionSave_triggered();
    }
}
