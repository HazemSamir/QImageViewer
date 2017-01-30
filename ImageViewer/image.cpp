#include "image.h"

Image::Image(QString path)
{
    QImage newImage;
    isLoaded = newImage.load(path);
    if (isLoaded)
        rotatedImage = image = newImage.copy();
    this->path = path;
}

QImage* Image::currentQImage()
{
    if (!isLoaded)
        return 0;
    propagate_rotation();
    return &rotatedImage;
}

double Image::rotate(double angle)
{
    if (!isLoaded)
        return 0;
    propagate_rotation();

    double delta = angle - rotation;
    if (angle != rotation) {
        rotation = angle;
        QMatrix rm;
        rm.rotate(angle);
        rotatedImage = image.transformed(rm, Qt::SmoothTransformation);
        if (rotation != 0)
            isRotated = true;
    }
    return delta;
}

/* record the angle without applying rotation */
double Image::lazy_rotate(double angle)
{
    if (!isLoaded)
        return 0;
    if (!lazy_rotated)
        lazy_rotation = rotation;
    double delta = angle - lazy_rotation;
    lazy_rotation = angle;
    lazy_rotated = true;
    return delta;
}

/* perform any recorded rotations if any */
void Image::propagate_rotation()
{
    if (lazy_rotated) {
        lazy_rotated = false;
        rotate(lazy_rotation);
        lazy_rotation = 0;
    }
}

QImage Image::crop(QRectF rect)
{
    if (!isLoaded)
        return QImage();
    propagate_rotation();
    isCropped = true;
    /// TODO: Handle if the crop rectangle out side the image rect
    image = rotatedImage.copy(rect.x(), rect.y(), rect.width(), rect.height());
    rotatedImage = image.copy();
    rotation = 0;
    lazy_rotation = 0;
    lazy_rotated = false;
    return image;
}

Image* Image::copy()
{
    Image* newImage = new Image();
    newImage->image = this->image.copy();
    newImage->rotatedImage = this->rotatedImage.copy();
    newImage->isCropped = this->isCropped;
    newImage->isLoaded = this->isLoaded;
    newImage->isRotated = this->isRotated;
    newImage->lazy_rotated = this->lazy_rotated;
    newImage->lazy_rotation = this->lazy_rotation;
    newImage->path = this->path;
    newImage->rotation = this->rotation;

    return newImage;
}
