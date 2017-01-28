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
public:
    enum Mode { NoMode,
        MovingMode,
        ZoomIn,
        Crop };
    double zoomOutFactor = 0.375;
    double zoomInFactor = 1.375;

    ImageScene(QObject* parent = 0);

    void setMode(Mode mode);
    Mode mode() { return sceneMode; }
    void setImage(Image* image);

    /// TODO: make it private
    QSlider* angleSlider;

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
