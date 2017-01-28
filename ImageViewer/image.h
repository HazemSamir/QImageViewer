#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>

class Image {
public:
    Image(QString path);

    QString imagePath() { return path; }

    int rotate(int angle);
    int lazy_rotate(int angle);
    int angle() { return rotation; }
    void propagate_rotation();

    bool changed() { return isRotated || isCropped; }
    bool loaded() { return isLoaded; }

    QImage crop(QRectF rect);

    QImage* currentQImage();

private:
    QString path;
    QImage image, rotatedImage;
    int rotation = 0;
    int lazy_rotation = 0;
    bool lazy_rotated = false;
    bool isRotated = false;
    bool isCropped = false;
    bool isLoaded = false;
};

#endif // IMAGE_H
