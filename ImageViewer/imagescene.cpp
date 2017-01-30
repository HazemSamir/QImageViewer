#include "imagescene.h"

ImageScene::ImageScene(QObject* parent)
    : QGraphicsScene(parent)
{
    sceneMode = NoMode;
    rect = 0;
    pixmapItem = 0;
}

ImageScene::~ImageScene()
{
    if (pixmapItem)
        delete pixmapItem;
}

void ImageScene::setMode(Mode mode)
{
    sceneMode = mode;
    QGraphicsView* mView = getGraphicsView();
    if (!mView)
        return;
    if (mode == Mode::ZoomIn) {
        mView->viewport()->setCursor(QCursor(QPixmap(":/cursors/cursors/zoomin_area.png")));
    } else if (mode == Mode::Crop) {
        mView->viewport()->setCursor(Qt::CrossCursor);
    } else if (mode == Mode::NoMode) {
        mView->viewport()->setCursor(Qt::OpenHandCursor);
        mView->setDragMode(QGraphicsView::NoDrag);
        removeSelectionRectangle();
    } else if (mode == Mode::MovingMode) {
        mView->viewport()->setCursor(Qt::ClosedHandCursor);
        mView->setDragMode(QGraphicsView::ScrollHandDrag);
        removeSelectionRectangle();
    }
    emit modeChanged(mode);
}

void ImageScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    origPoint = event->scenePos();
    if (sceneMode == Mode::NoMode) {
        setMode(Mode::MovingMode);
    }
    QGraphicsScene::mousePressEvent(event);
}

void ImageScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (!image || !image->loaded())
        return;
    if (sceneMode == Mode::NoMode)
        return;
    if (sceneMode == Mode::MovingMode) {
        return;
    } else {
        if (!rect) {
            rect = new QGraphicsRectItem;
            this->addItem(rect);
            rect->setPen(QPen(Qt::black, 1.0 / currentScaleValue(), Qt::SolidLine));
            rect->setRect(QRectF(origPoint, QSizeF(0, 0)));
        }
        QPointF topLeft(origPoint);
        QPointF bottomRight(event->scenePos());
        /* handle if the current point is not to the bottom left of the origpoint by swaping */
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
}

void ImageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsView* mView = getGraphicsView();
    if (!mView)
        return;
    if (!image || !image->loaded())
        return;
    if (sceneMode == Mode::ZoomIn && mView && rect) {
        int rectArea = rect->rect().width() * rect->rect().height();
        if (rectArea > 2)
            mView->fitInView(rect, Qt::KeepAspectRatio);
    } else if (sceneMode == Mode::Crop && rect) {
        int rectArea = rect->rect().width() * rect->rect().height();
        if (rectArea >= 1) {
            image->crop(rect->rect());
            updatePixmap();
            emit imageCropped();
        }
    }
    setMode(Mode::NoMode);
    QGraphicsScene::mouseReleaseEvent(event);
}

void ImageScene::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    if (!image || !image->loaded())
        return;
    event->ignore();
    QGraphicsView* mView = getGraphicsView();
    if (!mView)
        return;
    if (event->modifiers().testFlag(Qt::ControlModifier)) {
        if (event->delta() > 0) {
            mView->scale(zoomInFactor, zoomInFactor);
        } else if (event->delta() < 0) {
            mView->scale(zoomOutFactor, zoomOutFactor);
        }
        setMode(sceneMode);
        event->accept();
    }
}

void ImageScene::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
        setMode(Mode::NoMode);
}

void ImageScene::setImage(Image* image)
{
    this->image = image;
    updatePixmap();
}

void ImageScene::updatePixmap()
{
    if (!image || !image->loaded())
        return;

    if (pixmapItem) {
        removeItem(pixmapItem);
        delete pixmapItem;
    }

    QPixmap pixmap = QPixmap::fromImage(*image->currentQImage());
    pixmapItem = addPixmap(pixmap);
    setSceneRect(pixmap.rect());
}

double ImageScene::currentScaleValue()
{
    QGraphicsView* mView = getGraphicsView();
    if (!mView)
        return 1.0;
    return mView->transform().m11();
}

QGraphicsView* ImageScene::getGraphicsView()
{
    if (views().empty())
        return 0;
    return views().at(0);
}

void ImageScene::removeSelectionRectangle()
{
    if (rect) {
        removeItem(rect);
        delete rect;
        rect = 0;
    }
}
