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
    scene->setMode(ImageScene::NoMode);

#ifdef Q_OS_ANDROID
    ui->menuBar->removeAction(ui->menuEdit->menuAction());
    ui->menuBar->removeAction(ui->menuView->menuAction());
    ui->toolBar->removeAction(ui->actionOpen);
    ui->toolBar->removeAction(ui->actionSave);
#endif

    ui->rotationWidget->hide();
    connect(ui->actionRotate, SIGNAL(toggled(bool)), ui->rotationWidget, SLOT(setVisible(bool)));

    ui->zoomWidget->hide();
    connect(ui->actionZoomFactor, SIGNAL(toggled(bool)), ui->zoomWidget, SLOT(setVisible(bool)));

    connect(ui->angleHSlider, SIGNAL(valueChanged(int)), ui->angleSpinBox, SLOT(setValue(int)));
    connect(ui->angleSpinBox, SIGNAL(valueChanged(int)), ui->angleHSlider, SLOT(setValue(int)));

    connect(ui->zoomSlider, SIGNAL(valueChanged(int)), ui->factorSpinBox, SLOT(setValue(int)));
    connect(ui->factorSpinBox, SIGNAL(valueChanged(int)), ui->zoomSlider, SLOT(setValue(int)));

    connect(scene, SIGNAL(modeChanged(int)), this, SLOT(on_scene_modeChanged(int)));

    resize(QGuiApplication::primaryScreen()->availableSize() * 4 / 5);

/* for debugging */
#ifdef QT_DEBUG
    display(":/Images_list/Images/default");
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    save_changes();
    Q_UNUSED(event);
}

void MainWindow::on_toolBar_actionTriggered(QAction* a)
{
    propagate_lazy_rotate();
    Q_UNUSED(a);
}

void MainWindow::on_actionRotate_triggered(bool checked)
{
    scene->setMode(ImageScene::NoMode);
    ui->actionRotate->setChecked(checked);
    propagate_lazy_rotate();
}


void MainWindow::on_actionZoomFactor_triggered(bool checked)
{
    scene->setMode(ImageScene::NoMode);
    ui->actionZoomFactor->setChecked(checked);
    gv->scale(zoomFactor,zoomFactor);
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
    scene->setMode(ImageScene::NoMode);
    gv->scale(scene->zoomInFactor, scene->zoomInFactor);
    //gv->scale(2,2);
}

/* zoom out */
void MainWindow::on_actionZoomOut_triggered()
{
    scene->setMode(ImageScene::NoMode);
    gv->scale(scene->zoomOutFactor, scene->zoomOutFactor);
    //gv->scale(0.5,0.5);
}

void MainWindow::on_scene_modeChanged(int mode)
{
    ui->actionHandTool->setChecked(false);
    ui->actionZoomInArea->setChecked(false);
    ui->actionCrop->setChecked(false);
    ui->actionRotate->setChecked(false);
    ui->angleHSlider->setValue(image->angle());

    if (mode == ImageScene::Crop) {
        ui->actionCrop->setChecked(true);
    } else if (mode == ImageScene::ZoomIn) {
        ui->actionZoomInArea->setChecked(true);
    } else {
        ui->actionHandTool->setChecked(true);
    }
}

/* zoom in area */
void MainWindow::on_actionZoomInArea_triggered(bool checked)
{
    if (checked)
        scene->setMode(ImageScene::ZoomIn);
    else
        scene->setMode(ImageScene::NoMode);
}

/* crop */
void MainWindow::on_actionCrop_triggered(bool checked)
{
    if (checked)
        scene->setMode(ImageScene::Crop);
    else
        scene->setMode(ImageScene::NoMode);
}

/* hand tool */
void MainWindow::on_actionHandTool_triggered(bool checked)
{
    scene->setMode(ImageScene::NoMode);
    Q_UNUSED(checked);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
        scene->setMode(ImageScene::NoMode);
    else if (event->key() == Qt::Key_Back)
        qApp->quit();
}

/* reset */
void MainWindow::on_actionReset_triggered()
{
    if (!image || !image->loaded())
        return;
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

/*rotate slider */
void MainWindow::on_angleHSlider_valueChanged(int value)
{
    lazy_rotate(value);
}

/* angle rotate SpinBox */
void MainWindow::on_angleSpinBox_valueChanged(int value)
{
    lazy_rotate(value);
}

/*zoom slider */
void MainWindow::on_zoomSlider_valueChanged(int factor)
{
    zoomWithFactor(factor);
}

/* zoom factor SpinBox */
void MainWindow::on_factorSpinBox_valueChanged(double factor)
{
    zoomWithFactor(factor);
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
    if (image && image->loaded())
        scene->setImage(image);
}

void MainWindow::display(QString path)
{
    if (!image)
        delete image;
    image = new Image(path);
    if (!image || !image->loaded())
        return;
    this->setWindowTitle("Image Viewer::" + path);
    reset();
    gv->setScene(scene);
}

void MainWindow::reset()
{
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
    if (image->changed()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Save Changes", "Do you want to save changes?",
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
            on_actionSave_triggered();
    }
}

/*zoom with factor*/
void MainWindow::zoomWithFactor(double factor){

    if(factor != 0){
        if(factor < 0){factor = -1.0 / factor;}
        zoomFactor = factor;
        gv->scale(zoomFactor,zoomFactor);
    }
}
