#include "frame.h"
#include <QPixmap>
#include <QImage>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QColor>
#include <QPainter>
#include <QtSwap>

Frame::Frame(int sideLength, int pixelSize){
    foregroundPixmap = QPixmap(sideLength, sideLength);
    backgroundPixmap = QPixmap(sideLength, sideLength);

    foregroundPixmap.fill(Qt::transparent);
    updateBackgroundPixmap(sideLength, pixelSize);
}

Frame::Frame(const Frame& other){
    foregroundPixmap = QPixmap(other.foregroundPixmap);
    backgroundPixmap = QPixmap(other.backgroundPixmap);
}

Frame& Frame::operator =(Frame other){
    qSwap(foregroundPixmap, other.foregroundPixmap);
    qSwap(backgroundPixmap, other.backgroundPixmap);

    return *this;
}

QJsonObject Frame::ConvertToJson(){
    QImage image = foregroundPixmap.toImage();

    QJsonObject frame;
    frame["sideLength"] = image.height();

    QJsonArray pixelArray;

    for(int y = 0; y < image.height(); y++){
        for(int x = 0; x < image.width(); x++){
            QColor color = image.pixelColor(x,y);

            QJsonObject pixel;
            pixel["r"] = color.red();
            pixel["g"] = color.green();
            pixel["b"] = color.blue();
            pixel["a"] = color.alpha();

            pixelArray.append(pixel);
        }
    }


    frame["pixels"] = pixelArray;

    return frame;
}

void Frame::LoadFromJson(QJsonObject json){
    QJsonArray pixelArray = json["pixels"].toArray();
    int sideLength = json["sideLength"].toInt();

    QImage image(sideLength, sideLength, QImage::Format_ARGB32);

    int index = 0;
    for(int y = 0; y < sideLength; y++){
        for(int x = 0; x < sideLength; x++){
            QJsonObject pixel = pixelArray[index].toObject();

            int r = pixel["r"].toInt();
            int g = pixel["g"].toInt();
            int b = pixel["b"].toInt();
            int a = pixel["a"].toInt();

            QColor color(r,g,b,a);
            image.setPixelColor(x, y, color);
        }
    }

    foregroundPixmap = QPixmap::fromImage(image);
}

void Frame::resizePixmap(int newSideLength, int newPixelSize){
    sideLength = newSideLength;
    pixelSize = newPixelSize;
    int newResolution = newSideLength * newPixelSize;

    QPainter painter(&foregroundPixmap);
    painter.drawPixmap(0, 0, foregroundPixmap.scaled(newResolution, newResolution, Qt::IgnoreAspectRatio));

    updateBackgroundPixmap(newSideLength, newPixelSize);
}

void Frame::updateForegroundPixmap(QPoint pixelPos, QColor color){
    QPainter painter(&foregroundPixmap);

    painter.fillRect(pixelPos.x() * pixelSize, pixelPos.y() * pixelSize, pixelSize, pixelSize, color);
}

void Frame::updateBackgroundPixmap(int sideLength, int pixelSize){
    QColor black(117, 117, 117);
    QColor white(204, 204, 204);

    QPainter painter(&backgroundPixmap);
    for (int x = 0; x < sideLength; x++) {
        for (int y = 0; y < sideLength; y++) {
            int pixelOriginX = x * pixelSize;
            int pixelOriginY = y * pixelSize;
            bool isWhite = (x + y) % 2 == 1;

            painter.fillRect(pixelOriginX, pixelOriginY, pixelSize, pixelSize, isWhite ? white : black);
        }
    }
}
