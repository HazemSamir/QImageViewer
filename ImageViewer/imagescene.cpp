#include "imagescene.h"

ImageScene::ImageScene(QObject* parent): QGraphicsScene(parent)
{
    sceneMode = NoMode;
    rect = 0;
    pixmapItem = 0;
}

void ImageScene::setMode(Mode mode){
    sceneMode = mode;
    if (views().empty())
        return;
    QGraphicsView* mView = views().at(0);
    if (mode == Mode::ZoomIn) {
        mView->viewport()->setCursor(QCursor(QPixmap(":/cursors/cursors/zoomin.png")));
    } else if (mode == Mode::Crop){
        mView->viewport()->setCursor(Qt::CrossCursor);
    }
}

void ImageScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    origPoint = event->scenePos();
    QGraphicsScene::mousePressEvent(event);
}

void ImageScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(!rect){
        rect = new QGraphicsRectItem;
        this->addItem(rect);
        rect->setPen(QPen(Qt::black, 1, Qt::SolidLine));
        rect->setRect(QRectF(origPoint, QSizeF(0, 0)));
    }
    QPointF topLeft(origPoint);
    QPointF bottomRight(event->scenePos());
    if (topLeft.x() > bottomRight.x()) {
        int temp = topLeft.x();
        topLeft.setX(bottomRight.x());
        bottomRight.setX(temp);
    }
    if (topLeft.y() > bottomRight.y()) {
        int temp = topLeft.y();
        topLeft.setY(bottomRight.y());
        bottomRight.setY(temp);
    }
    rect->setRect(QRectF(topLeft, bottomRight));
}

void ImageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsView* mView = views().at(0);
    if (sceneMode == Mode::ZoomIn) {
        if(mView && rect && rect->rect().width() && rect->rect().height())
            mView->fitInView(rect, Qt::KeepAspectRatio);
    } else if (sceneMode == Mode::Crop) {
        if (rect && rect->rect().width() && rect->rect().height()) {
            image->crop(rect->rect());
            updatePixmap();
            if (angleSlider)
                angleSlider->setValue(0);
        }
        setMode(Mode::ZoomIn);
    }

    removeItem(rect);
    delete rect;
    rect = 0;
    QGraphicsScene::mouseReleaseEvent(event);
}

void ImageScene::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Delete)
        foreach(QGraphicsItem* item, selectedItems()){
            removeItem(item);
            delete item;
        }
    else
        QGraphicsScene::keyPressEvent(event);
}

void ImageScene::setImage(Image *image) {
    this->image = image;
    updatePixmap();
}

void ImageScene::updatePixmap() {
    if (!image)
        return;

    if(pixmapItem) {
      removeItem(pixmapItem);
      delete pixmapItem;
    }

    QPixmap pixmap = QPixmap::fromImage(*image->currentQImage());
    pixmapItem = addPixmap(pixmap);
    setSceneRect(pixmap.rect());
}
