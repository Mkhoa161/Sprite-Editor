#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPixmap>
#include <QPainter>
#include <QColor>
#include <QPoint>
#include <QRect>
#include <QPainter>

namespace Ui {
class Canvas;
}

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();
    void paint(QPoint pixelPos, QColor color);



private:
    Ui::Canvas *ui;

    int pixelSize;
    QPoint mousePixelPos;

    QPixmap backgroundPixmap;
    QPixmap foregroundPixmap;

    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

    QRect getPixelRect(QPoint pixelPos);
    QPoint convertWorldToPixel(QPoint mousePos);
    void paintCheckerBoard();
};

#endif // CANVAS_H
