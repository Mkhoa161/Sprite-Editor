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

private:
    enum Mode {
        BRUSH,
        ERASER,
        CIRCLE,
        CIRCLEFILLED,
        SQUARE,
        SQUAREFILLED,
        TRIANGLE,
        TRIANGLEFILLED
    };

    //Temp value, will change based on decided default canvas size
    const int DEFAULT_PIXEL_SIZE = 50;

    const QColor DEFAULT_COLOR = Qt::black;
    const enum Mode DEFAULT_MODE = BRUSH;

    Ui::Canvas *ui;

    int pixelSize;
    QPoint mousePixelPos;
    bool isPressingMouse;

    QColor selectedColor;

    QPixmap backgroundPixmap;
    QPixmap foregroundPixmap;

    enum Mode currentMode;
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

public:

    explicit Canvas(QWidget *parent = nullptr);

    ~Canvas();

    void paint(QPoint pixelPos);

    void selectTool(enum Mode newMode);

    void setCurrentColor(int r, int g, int b, int a);
};

#endif // CANVAS_H
