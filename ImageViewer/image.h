#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>

class Image {
public:
    Image(QString path);

    QString imagePath() { return path; }

    double rotate(double angle);
    double lazy_rotate(double angle);
    double angle() { propagate_rotation(); return rotation; }
    double lazy_angle() { return lazy_rotation; }

    bool changed() { propagate_rotation(); return isRotated || isCropped; }
    bool loaded() { return isLoaded; }

    QImage crop(QRectF rect);

    QImage* currentQImage();

    Image *copy();

private:
    Image() {};

    QString path;
    QImage image, rotatedImage;
    double rotation = 0;
    double lazy_rotation = 0;
    bool lazy_rotated = false;
    bool isRotated = false;
    bool isCropped = false;
    bool isLoaded = false;

    void propagate_rotation();
};

#endif // IMAGE_H
