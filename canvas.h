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
#include "frame.h"

namespace Ui {
class Canvas;
}

class Canvas : public QWidget
{
    Q_OBJECT

public:
    enum Mode {
        BRUSH = 0,
        ERASER = 1,
        CIRCLE = 2,
        CIRCLEFILLED = 3,
        SQUARE = 4,
        SQUAREFILLED = 5,
        TRIANGLE = 6,
        TRIANGLEFILLED = 7
    };

    const QColor DEFAULT_COLOR = Qt::black;
    const enum Mode DEFAULT_MODE = BRUSH;

    explicit Canvas(QWidget *parent = nullptr);

    ~Canvas();

signals:
    void paint(QPoint pixelPos, QColor color);

public slots:
    void selectTool(enum Mode newMode);

    void setCurrentColor(int r, int g, int b, int a);

    void onSelectedFrameChanged(Frame* newSelectedFrame);

private:
    //Temp value, will change based on decided default canvas size
    const int DEFAULT_PIXEL_SIZE = 50;

    Ui::Canvas *ui;

    int pixelSize;
    QPoint mousePixelPos;
    bool isPressingMouse;

    QColor selectedColor;

    QPixmap backgroundPixmap;
    QPixmap* foregroundPixmap;

    enum Mode currentMode = DEFAULT_MODE;
    bool isMirrorMode;

    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    //Not sure if we'll end up needing this so I just commented it out for now
    // void leaveEvent(QEvent *event) override;

    QRect getPixelRect(QPoint pixelPos);

    QPoint convertWorldToPixel(QPoint mousePos);

    void paintCheckerBoard();

};

#endif // CANVAS_H
