#include "image.h"

Image::Image(QString path)
{
    QImage newImage;
    newImage.load(path);
    rotatedImage = image = newImage.copy();
    this->path = path;
}

QImage *Image::currentQImage() {
    propagate_rotation();
    return &rotatedImage;
}

int Image::rotate(int angle) {
    int delta = angle - rotation;

    rotation = angle;
    lazy_rotation = angle;
    
    QMatrix rm;
    rm.rotate(angle);
    rotatedImage = image.transformed(rm, Qt::SmoothTransformation);
    
    return delta;
}

int Image::lazy_rotate(int angle) {
    int delta = angle - lazy_rotation;
    lazy_rotation = angle;
    lazy_rotated = true;
    return delta;
}

void Image::propagate_rotation() {
    if (lazy_rotated) {
        rotate(lazy_rotation);
        lazy_rotated = false;
    }
}

QImage Image::crop(QRectF rect) {
    propagate_rotation();
    rotatedImage = image = rotatedImage.copy(rect.x(), rect.y(), rect.width(), rect.height());
    rotation = 0;
    lazy_rotation = 0;
    lazy_rotated = false;
    return image;
}
