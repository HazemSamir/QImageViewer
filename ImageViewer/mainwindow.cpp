#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QPixmap>
#include <QScrollArea>
#include <QTransform>
#include <QtGui>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gv = ui->graphicsView;
    scene = new ImageScene(this);
    scene->angleSlider = ui->angleHSlider;
    scene->setMode(ImageScene::NoMode);

    ui->rotationWidget->hide();
    connect(ui->actionRotate, SIGNAL(toggled(bool)), ui->rotationWidget, SLOT(setVisible(bool)));

    connect(ui->angleHSlider, SIGNAL(valueChanged(int)), ui->angleSpinBox, SLOT(setValue(int)));
    connect(ui->angleSpinBox, SIGNAL(valueChanged(int)), ui->angleHSlider, SLOT(setValue(int)));

    resize(QGuiApplication::primaryScreen()->availableSize() * 4 / 5);

    /* for debugging */
    #ifdef QT_DEBUG
    display(":/Images_list/Images/default");
    #endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    save_changes();
    Q_UNUSED(event);
}

/* open */
void MainWindow::on_actionOpen_triggered()
{
    propagate_lazy_rotate();
    scene->setMode(ImageScene::NoMode);
    save_changes();
    QString imagePath = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "",
        tr("Image (*.jpg *.jpeg *.png *.bmp)"));
    if (imagePath.size() == 0)
        return;
    reset();
    display(imagePath);
}

/* save */
void MainWindow::on_actionSave_triggered()
{
    if (!image || !image->loaded())
        return;
    propagate_lazy_rotate();
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
    propagate_lazy_rotate();
    scene->setMode(ImageScene::NoMode);
    gv->scale(scene->zoomInFactor, scene->zoomInFactor);
}

/* zoom out */
void MainWindow::on_actionZoomOut_triggered()
{
    propagate_lazy_rotate();
    scene->setMode(ImageScene::NoMode);
    gv->scale(scene->zoomOutFactor, scene->zoomOutFactor);
}

/* zoom in area */
void MainWindow::on_actionZoomInArea_triggered(bool checked)
{
    propagate_lazy_rotate();
    gv->rotate(-image->lazy_angle());
    scene->setImage(image);
    if (checked) {
        scene->setMode(ImageScene::ZoomIn);
        ui->actionHandTool->setChecked(false);
        ui->actionCrop->setChecked(false);
    } else {
        scene->setMode(ImageScene::NoMode);
        ui->actionHandTool->setChecked(true);
        ui->actionCrop->setChecked(false);
    }
}

/* crop */
void MainWindow::on_actionCrop_triggered(bool checked)
{
    propagate_lazy_rotate();
    if (checked) {
        scene->setMode(ImageScene::Crop);
        ui->actionHandTool->setChecked(false);
        ui->actionZoomInArea->setChecked(false);
    } else {
        scene->setMode(ImageScene::NoMode);
        ui->actionHandTool->setChecked(true);
        ui->actionZoomInArea->setChecked(false);
    }
}

/* hand tool */
void MainWindow::on_actionHandTool_triggered(bool checked)
{
    propagate_lazy_rotate();
    scene->setMode(ImageScene::NoMode);
    ui->actionHandTool->setChecked(true);
    ui->actionZoomInArea->setChecked(false);
    ui->actionCrop->setChecked(false);
    Q_UNUSED(checked);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    propagate_lazy_rotate();
    if (event->key() == Qt::Key_Escape) {
        on_actionHandTool_triggered(true);
        ui->actionRotate->setChecked(false);
    }
}

/* reset */
void MainWindow::on_actionReset_triggered()
{
    if (!image || !image->loaded())
        return;
    propagate_lazy_rotate();
    if (image->changed()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Warning", "Changes will be lost, are you sure you want to reset?",
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
            display(image->imagePath());
    } else {
        reset();
    }
}

/* slider */
void MainWindow::on_angleHSlider_valueChanged(int value)
{
    lazy_rotate(value);
}

/* angleSpinBox */
void MainWindow::on_angleSpinBox_valueChanged(int value)
{
    lazy_rotate(value);
}

/*----------- utils functions -------- */

void MainWindow::lazy_rotate(int value)
{
    if (!image || !image->loaded())
        return;
    int delta = image->lazy_rotate(value);
    gv_lazy_rotation += delta;
    gv->rotate(delta);
}

void MainWindow::propagate_lazy_rotate()
{
    gv->rotate(-gv_lazy_rotation);
    gv_lazy_rotation = 0;
    if (image)
        scene->setImage(image);
}

void MainWindow::display(QString path)
{
    if (!image)
        delete image;
    propagate_lazy_rotate();
    image = new Image(path);
    if (!image || !image->loaded())
        return;
    this->setWindowTitle("Image Viewer::" + path);
    reset();
    gv->setScene(scene);
}

void MainWindow::reset()
{
    propagate_lazy_rotate();
    ui->angleHSlider->setValue(0);
    gv->resetMatrix();
    if (!image || !image->loaded())
        return;
    scene->setImage(image);
    scene->setMode(ImageScene::NoMode);
}

void MainWindow::save_changes()
{
    if (!image || !image->loaded())
        return;
    propagate_lazy_rotate();
    if (image->changed()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Save Changes", "Do you want to save changes?",
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
            on_actionSave_triggered();
    }
}
