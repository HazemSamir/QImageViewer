#ifndef IMAGESCENE_H
#define IMAGESCENE_H

#include "image.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QAction>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QSlider>

class ImageScene : public  QGraphicsScene
{
public:
    enum Mode {NoMode, ZoomIn, Crop};

    ImageScene(QObject* parent = 0);
    
    void setMode(Mode mode);
    void setImage(Image *image);

    QSlider *angleSlider;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    
private:
    Mode sceneMode;
    QPointF origPoint;
    QGraphicsRectItem* rect;
    
    Image *image;

    QGraphicsPixmapItem *pixmapItem;

    void updatePixmap();
};


#endif // IMAGESCENE_H
