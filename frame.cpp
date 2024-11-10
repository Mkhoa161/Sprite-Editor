#include "frame.h"
#include <QPixmap>
#include <QImage>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QColor>
#include <QPainter>
#include <QtSwap>

#include <QDebug>
#include <QString>

Frame::Frame(int sideLength, int pixelSize){
    pixmap = QPixmap(sideLength, sideLength);
    pixmap.fill(Qt::transparent);

    this->sideLength = sideLength;
    this->pixelSize = pixelSize;
}

Frame::Frame(const Frame& other){
    pixmap = QPixmap(other.pixmap);
}

Frame& Frame::operator =(Frame other){
    qSwap(pixmap, other.pixmap);

    return *this;
}

QJsonObject Frame::ConvertToJson(){
    QImage image = pixmap.toImage();

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

    pixmap = QPixmap::fromImage(image);
}

void Frame::resizePixmap(int newSideLength, int newPixelSize){
    sideLength = newSideLength;
    pixelSize = newPixelSize;
    int newResolution = newSideLength;

    QPainter painter(&pixmap);
    painter.drawPixmap(0, 0, pixmap.scaled(newResolution, newResolution, Qt::IgnoreAspectRatio));
}

void Frame::updatePixmap(QPoint pixelPos, QColor color){
    QPainter painter(&pixmap);

    painter.fillRect(pixelPos.x(), pixelPos.y(), pixelSize, pixelSize, color);

    QString formattedJsonStr = QString::fromUtf8(QJsonDocument(ConvertToJson()).toJson(QJsonDocument::Indented)).replace("\n", "").replace("    ", " ");
    //qDebug() << formattedJsonStr;
}
