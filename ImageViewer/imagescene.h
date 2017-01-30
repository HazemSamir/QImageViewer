#ifndef IMAGESCENE_H
#define IMAGESCENE_H

#include "image.h"

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QSlider>

class ImageScene : public QGraphicsScene {

Q_OBJECT

public:
    enum Mode { NoMode,
        MovingMode,
        ZoomIn,
        Crop };
    double zoomOutFactor = 0.5;
    double zoomInFactor = 1.375;

    ImageScene(QObject* parent = 0);
    ~ImageScene();

    void setMode(Mode mode);
    Mode mode() { return sceneMode; }
    void setImage(Image* image);

signals:
    void modeChanged(int);
    void imageCropped();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void wheelEvent(QGraphicsSceneWheelEvent* event);

private:
    Mode sceneMode;
    QPointF origPoint;
    QGraphicsRectItem* rect;

    Image* image;
    QGraphicsPixmapItem* pixmapItem;

    void updatePixmap();
    void removeSelectionRectangle();
    double currentScaleValue();
    QGraphicsView* getGraphicsView();
};

#endif // IMAGESCENE_H
