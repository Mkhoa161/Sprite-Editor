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
#include <vector>

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
    void erase(QPoint pixelPos);

public slots:
    void selectTool(enum Mode newMode);

    void setCurrentColor(int r, int g, int b, int a);

    void setMirrorMode(bool enabled);

    void onSelectedFrameChanged(Frame* newSelectedFrame);
    void onSideLengthChanged(int newSideLength);

private:
    //Temp value, will change based on decided default canvas size
    const int DEFAULT_PIXEL_SIZE = 50;

    Ui::Canvas *ui;

    int sideLength;
    int pixelSize;

    QPoint mousePixelPos;
    bool isPressingMouse;

    QColor selectedColor;

    QPixmap backgroundPixmap;
    QPixmap* foregroundPixmap;

    std::vector<QPoint> paintedPixels;
    std::vector<QColor> paintedColors;

    enum Mode currentMode = DEFAULT_MODE;

    bool isMirrorMode;
    bool isShapeMode;

    std::vector<QPoint> shapePixels;

    QPoint shapeStartPos;

    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void setShapeMode(bool enabled);
    void moveAndDisplayPixels(QColor color);
    void redrawShape();

    QRect getPixelRect(QPoint pixelPos);

    QPoint convertWorldToPixel(QPoint mousePos);

    void paintPixels();

    void paintCheckerBoard();

    std::vector<QPoint> mirrorPixels(std::vector<QPoint> pixelPositions);

};

#endif // CANVAS_H
