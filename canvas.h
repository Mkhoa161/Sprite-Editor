/*
    Authors: Zhuyi Bu, Zhenzhi Liu, Justin Melore, Maxwell Rodgers, Duke Nguyen, Minh Khoa Ngo
    Github usernames: 1144761429, 0doxes0, JustinMelore, maxdotr, duke7012, Mkhoa161
    Date: November 12th, 2024
    Class: CS3505, Fall 2024
    Assignment - A8: Sprite Editor Implementation

    The Canvas class is a custom QWidget used within the Sprite Editor application, enabling users to paint, erase,
    and manipulate pixels within a designated drawing area. As the primary editing surface in the view,
    Canvas allows for pixel specific changes, supporting various tools, shapes, and colors
    to create and modify individual frames in the animation sequence.

    Code style checked by: Zhuyi Bu
*/

#ifndef CANVAS_H
#define CANVAS_H

#include "frame.h"
#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QColor>
#include <QPoint>
#include <vector>

using std::vector;

namespace Ui {
class Canvas;
}

class Canvas : public QWidget {
    Q_OBJECT

public:
    /// \brief Enumeration for the different drawing modes Canvas uses to draw pixels on the widget.
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

    /// \brief Canvas consturctor to initialize the custom drawing widget.
    explicit Canvas(QWidget *parent = nullptr);

    /// \brief Canvas destructor.
    ~Canvas();

signals:
    void painted(QPoint pixelPos, QColor color);
    void erased(QPoint pixelPos);

public slots:
    /// \brief Slot to capture when the user selects a different tool mode.
    /// Sets the selected tool to the new mode.
    /// \param newMode The mode type of the selected tool.
    void onToolSelected(enum Mode newMode);

    /// \brief Slot to capture when the user selects a different color.
    /// Sets the selected color to the color with selected RGBA values.
    /// \param r The red value of the new color.
    /// \param g The green value of the new color.
    /// \param b The blue value of the new color.
    /// \param a The alpha value of the new color.
    void onCurrentColorSet(int r, int g, int b, int a);

    /// \brief Slot to capture when the user turns on mirror mode functionality of the canvas.
    /// When mirror mode functionality is on, drawn pixels are reflected across the Y axis of the canvas.
    /// \param enabled Whether mirror mode is enabled.
    void onMirrorModeSet(bool enabled);

    /// \brief Slot to capture when the user changes the selected frame to work on.
    /// The pixels drawn on the canvas will be repainted to reflect the selected frame.
    /// \param newSelectedFrame A reference to the newly selected frame.
    void onSelectedFrameChanged(Frame* newSelectedFrame);

    /// \brief Slot to capture when the user changes the sidelength of the canvas.
    /// The canvas will be redrawn to reflect the newly selected dimensions.
    void onSideLengthChanged(int newSideLength);

private:
    const int DEFAULT_PIXEL_SIZE = 50;

    Ui::Canvas *ui;

    int uiMinSide = 500;
    int sideLength;
    int pixelSize;
    int canvasSize;

    QPoint mousePixelPos;
    bool isPressingMouse;

    QColor selectedColor;

    QPixmap backgroundPixmap;
    QPixmap* foregroundPixmap;

    vector<QPoint> paintedPixels;
    vector<QColor> paintedColors;

    enum Mode currentMode = DEFAULT_MODE;

    bool isMirrorMode;
    bool isShapeMode;

    vector<QPoint> shapePixels;

    QPoint shapeStartPos;

    /// \brief Overriden paintEvent to draw the backing pixmap to the canvas.
    /// \param A pointer to the QPaintEvent which provides information about drawable widget.
    void paintEvent(QPaintEvent *event) override;

    /// \brief Overriden mouseMoveEvent to capture painting mouse events.
    /// \param event A pointer to the QMouseMoveEvent to provide information about mouse behaviour.
    void mouseMoveEvent(QMouseEvent *event) override;

    //// \brief Overriden mousePressEvent to capture painting events, particularly used for shape tools
    /// to capture the starting position for the shape.
    /// \param event A pointer to the QMouseEvent to provide information about mouse behaviour.
    void mousePressEvent(QMouseEvent *event) override;

    /// \brief Overriden mouseReleaseEvent to capture painting behavious, particularly used for shaep tools
    /// to capture the ending position of the shape.
    /// \param event A pointer to the QMouseEvent to provide information about mouse behaviour.
    void mouseReleaseEvent(QMouseEvent *event) override;

    /// \brief Shape tools helper method that paints the pixels of the shape to the screen permanently.
    /// \param color The color of the shape pixels.
    void moveAndDisplayPixels(QColor color);

    /// \brief Shape tools helper method that redraws the shape with the new positions when the mouse moves.
    void redrawShape();

    /// \brief Paint pixels to the screen defined by user actions, tool selection, and color selection.
    void paintPixels();

    /// \brief Helper method for paintPixels when BRUSH mode is selected.
    /// \param color The color of the brush.
    void brushPainting(QColor color);

    /// \brief Helper method for the paintPixels when ERASER mode is selected.
    /// \param color The color of the eraser.
    void eraserPainting(QColor color);

    /// \brief Helper method for the paintPixels when the SQUARE mode is selected.
    /// \param color The color of the square.
    void squarePainting(QColor color);

    /// \brief Helper method for the paintPixels when the SQUAREFILLED mode is selected.
    /// \param color The color of the filled square.
    void squareFilledPainting(QColor color);

    /// \brief Helper method for the paintPixels when the CIRCLE mode is selected.
    /// \param color The color of the circle.
    void circlePainting(QColor color);

    /// \brief Helper method for the paintPixels when the CIRCLEFILLED mode is selected.
    /// \param color The color of the filled circle.
    void circleFilledPainting(QColor color);

    /// \brief Helper method for the paintPixels when the TRIANGLE mode is selected.
    /// \param color The color of the triangle.
    void trianglePainting(QColor color);

    /// \brief Helper method for the paintPixels when the TRIANGLEFILLED mode is selected.
    /// \param color The color of the filled triangle.
    void triangleFilledPainting(QColor color);

    /// \brief Converts the XY position of the mouse in world space to a point on the canvas for use in selecting specific pixels.
    /// \param mousePos The position of the mouse.
    QPoint convertWorldToPixel(QPoint mousePos);

    /// \brief Draws the background pixmap as a checkerboard signaling transparent pixels to the user.
    void paintCheckerBoard(int resolution);

    /// \brief Draws a set of pixels reflected across the Y axis.
    /// \param pixelPositions The position of the pixels to be reflected across the Y axis.
    QPoint mirrorPixel(QPoint pixelPosition);

    /// \brief Overriden resizeEvent of QWidger, to redraw the canvas to fit inside the UI.
    /// \param event QT event parent.
    virtual void resizeEvent(QResizeEvent *event) override;

};

#endif // CANVAS_H
