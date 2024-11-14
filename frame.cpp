/*
    Authors: Zhuyi Bu, Zhenzhi Liu, Justin Melore, Maxwell Rodgers, Duke Nguyen, Minh Khoa Ngo
    Github usernames: 1144761429, 0doxes0, JustinMelore, maxdotr, duke7012, Mkhoa161
    Date: November 8th, 2024
    Class: CS3505, Fall 2024
    Assignment - A8: Sprite Editor Implementation

    The Frame class serves as the model that stores the data of a frame in the sprite. It has provides some functions
    that helps to manipulate the data. For example, export the data to json, rotate the frame by 90 degrees, paint at
    a specified pixel, etc.

    Code style checked by: Maxwell Rodgers
*/

#include "frame.h"
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <QtSwap>

Frame::Frame(int sideLength) {
    pixmap = QPixmap(sideLength, sideLength);
    pixmap.fill(Qt::transparent);
    this->sideLength = sideLength;
}

Frame::Frame(const Frame& other) {
    pixmap = QPixmap(other.pixmap);
}

Frame& Frame::operator=(Frame other) {
    qSwap(pixmap, other.pixmap);
    return *this;
}

QJsonArray Frame::convertToJson() {
    QImage image = pixmap.toImage();
    QJsonArray pixelArrayJson;

    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            QColor color = image.pixelColor(x,y);

            QJsonObject pixel;
            pixel["r"] = color.red();
            pixel["g"] = color.green();
            pixel["b"] = color.blue();
            pixel["a"] = color.alpha();

            pixelArrayJson.append(pixel);
        }
    }

    return pixelArrayJson;
}

void Frame::loadFromJson(QJsonValue json) {
    QJsonArray pixelArray = json.toArray();
    QImage image(sideLength, sideLength, QImage::Format_ARGB32);

    int index = 0;
    for (int y = 0; y < sideLength; y++) {
        for (int x = 0; x < sideLength; x++) {
            QJsonObject pixel = pixelArray[index].toObject();

            int r = pixel["r"].toInt();
            int g = pixel["g"].toInt();
            int b = pixel["b"].toInt();
            int a = pixel["a"].toInt();

            QColor color(r,g,b,a);
            image.setPixelColor(x, y, color);
            index++;
        }
    }

    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawPixmap(0, 0, QPixmap::fromImage(image));
}

void Frame::resizePixmap(int newSideLength) {
    QPixmap newPixmap(newSideLength, newSideLength);
    newPixmap.fill(Qt::transparent);
    QPainter painter(&newPixmap);
    painter.drawPixmap(0, 0, pixmap);
    qSwap(pixmap, newPixmap);
}

void Frame::updatePixmap(QPoint pixelPos, QColor color) {
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(pixelPos.x(), pixelPos.y(), 1, 1, color);
}

void Frame::rotate(bool isClockwise) {
    QTransform transform;
    transform.rotate(isClockwise ? 90 : -90);
    pixmap = pixmap.transformed(transform);
}

void Frame::flip(bool isAlongXAxis) {
    QTransform transform;

    if (isAlongXAxis) {
        transform.scale(1, -1);
        transform.translate(0, -pixmap.height());
    } else {
        transform.scale(-1, 1);
        transform.translate(-pixmap.width(), 0);
    }

    pixmap = pixmap.transformed(transform);
}
