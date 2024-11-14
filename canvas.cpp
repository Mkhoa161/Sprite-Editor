/*
    The Canvas class is a custom QWidget used within the Sprite Editor application, enabling users to paint, erase,
    and manipulate pixels within a designated drawing area. As the primary editing surface in the view,
    Canvas allows for pixel specific changes, supporting various tools, shapes, and colors
    to create and modify individual frames in the animation sequence.

    Code style checked by: Zhuyi Bu
*/

#include "canvas.h"
#include "ui_canvas.h"
#include <QMouseEvent>
#include <QPainter>
#include <vector>

using std::vector;

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Canvas) {
    ui->setupUi(this);

    pixelSize = DEFAULT_PIXEL_SIZE;
    selectedColor = DEFAULT_COLOR;
    currentMode = DEFAULT_MODE;

    isMirrorMode = false;
    isShapeMode = false;

    backgroundPixmap = QPixmap(size());
}

Canvas::~Canvas() {
    delete ui;
}

void Canvas::onToolSelected(Mode mode) {
    currentMode = mode;

    switch(currentMode){
        case BRUSH:
            isShapeMode = false;
            break;
        case ERASER:
            isShapeMode = false;
            break;
        default:
            isShapeMode = true;
    }
}

void Canvas::onMirrorModeSet(bool enabled) {
    isMirrorMode = enabled;
}

void Canvas::onCurrentColorSet(int r, int g, int b, int a) {
    selectedColor = QColor(r, g, b, a);
}

void Canvas::onSelectedFrameChanged(Frame *newSelectedFrame) {
    foregroundPixmap = &(newSelectedFrame->pixmap);

    paintedPixels.clear();
    paintedColors.clear();

    repaint();
}

void Canvas::onSideLengthChanged(int newSideLength) {
    sideLength = newSideLength;
    pixelSize = uiMinSide / sideLength;
    paintCheckerBoard(pixelSize * sideLength);
    repaint();
}

void Canvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.drawPixmap(0, 0, backgroundPixmap);

    if (foregroundPixmap != nullptr) {
        int scaledResolution = foregroundPixmap->height() * pixelSize;
        painter.drawPixmap(0, 0, foregroundPixmap->scaled(scaledResolution, scaledResolution));
    }
}

QPoint Canvas::mirrorPixel(QPoint pixelPosition) {
    int newPosition = canvasSize / pixelSize - pixelPosition.x() - 1;
    return QPoint(newPosition, pixelPosition.y());
}

void Canvas::paintPixels() {
    // Make sure the canvas size is fixed for calculation in Mirror Mode
    canvasSize = pixelSize * sideLength;

    // Set the current chosen color based on the mode
    QColor color = (currentMode == ERASER) ? Qt::transparent : selectedColor;

    // Select and execute the appropriate painting action based on the current mode
    switch (currentMode) {
        case BRUSH:
            brushPainting(color);
            break;
        case ERASER:
            eraserPainting(color);
            break;
        case SQUARE:
            squarePainting(color);
            break;
        case SQUAREFILLED:
            squareFilledPainting(color);
            break;
        case CIRCLE:
            circlePainting(color);
            break;
        case CIRCLEFILLED:
            circleFilledPainting(color);
            break;
        case TRIANGLE:
            trianglePainting(color);
            break;
        case TRIANGLEFILLED:
            triangleFilledPainting(color);
            break;
        default:
            break;
    }

    // Trigger a repaint to apply changes
    repaint();
}

void Canvas::moveAndDisplayPixels(QColor color) {
    for (int i = 0; i < int(paintedPixels.size()); i++) {
        QPoint currPixel = paintedPixels[i];
        QColor currColor = paintedColors[i];
        emit painted(currPixel, currColor);
    }

    for (QPoint pixel : shapePixels) {
        paintedPixels.push_back(pixel);
        paintedColors.push_back(color);
        emit painted(pixel, color);
    }
    shapePixels.clear();
}

void Canvas::redrawShape() {
    for (QPoint shapePixel : shapePixels) {
        emit painted(shapePixel, Qt::transparent);
    }

    for (int i = 0; i < int(paintedPixels.size()); i++) {
        QPoint currPixel = paintedPixels[i];
        QColor currColor = paintedColors[i];
        emit painted(currPixel, currColor);
    }
}

void Canvas::brushPainting(QColor color) {
    paintedPixels.push_back(QPoint(mousePixelPos));
    paintedColors.push_back(color);

    if (isMirrorMode) {
        QPoint mirroredPixel = mirrorPixel(mousePixelPos);
        paintedPixels.push_back(mirroredPixel);
        paintedColors.push_back(color);
        emit painted(mirroredPixel, color);
    }

    emit painted(mousePixelPos, color);
}

void Canvas::eraserPainting(QColor color) {
    vector<QPoint> newPaintedPixels;
    vector<QColor> newPaintedColors;

    for (int i = 0; i < int(paintedPixels.size()); i++) {
        QPoint currPixel = paintedPixels[i];
        QColor currColor = paintedColors[i];
        if (!(currPixel.x() == mousePixelPos.x() && currPixel.y() == mousePixelPos.y())) {
            if (isMirrorMode) {
                QPoint mirroredPixel = mirrorPixel(currPixel);
                if (!(mirroredPixel.x() == mousePixelPos.x() && mirroredPixel.y() == mousePixelPos.y())) {
                    newPaintedPixels.push_back(currPixel);
                    newPaintedColors.push_back(currColor);
                }
            } else {
                newPaintedPixels.push_back(currPixel);
                newPaintedColors.push_back(currColor);
            }
        }
    }

    paintedPixels.clear();
    paintedColors.clear();

    for (int i = 0; i < int(newPaintedPixels.size()); i++) {
        paintedPixels.push_back(newPaintedPixels[i]);
        paintedColors.push_back(newPaintedColors[i]);
    }

    emit painted(QPoint(mousePixelPos), color);

    if (isMirrorMode) {
        emit painted(mirrorPixel(mousePixelPos), color);
    }
}

void Canvas::squarePainting(QColor color) {

    redrawShape();

    if (isPressingMouse) {
        shapePixels.clear();

        QPoint startPixel = shapeStartPos;
        QPoint endPixel = mousePixelPos;

        int x1 = std::min(startPixel.x(), endPixel.x());
        int y1 = std::min(startPixel.y(), endPixel.y());
        int x2 = std::max(startPixel.x(), endPixel.x());
        int y2 = std::max(startPixel.y(), endPixel.y());

        // Top and bottom edges
        for (int i = x1; i <= x2; ++i) {
            shapePixels.push_back(QPoint(i, y1)); // Top edge
            shapePixels.push_back(QPoint(i, y2)); // Bottom edge

            if (isMirrorMode) {
                shapePixels.push_back(mirrorPixel(QPoint(i, y1)));
                shapePixels.push_back(mirrorPixel(QPoint(i, y2)));
            }
        }

        // Left and right edges
        for (int j = y1 + 1; j < y2; ++j) {
            shapePixels.push_back(QPoint(x1, j)); // Left edge
            shapePixels.push_back(QPoint(x2, j)); // Right edge

            if (isMirrorMode) {
                shapePixels.push_back(mirrorPixel(QPoint(x1, j)));
                shapePixels.push_back(mirrorPixel(QPoint(x2, j)));
            }
        }

        for (QPoint pixel : shapePixels) emit painted(pixel, color);
    } else {
        moveAndDisplayPixels(color);
    }
}

void Canvas::squareFilledPainting(QColor color) {

    redrawShape();

    if (isPressingMouse) {
        shapePixels.clear();

        QPoint startPixel = shapeStartPos;
        QPoint endPixel = mousePixelPos;

        int xMin = std::min(startPixel.x(), endPixel.x());
        int xMax = std::max(startPixel.x(), endPixel.x());
        int yMin = std::min(startPixel.y(), endPixel.y());
        int yMax = std::max(startPixel.y(), endPixel.y());

        for (int i = xMin; i <= xMax; i++) {
            for (int j = yMin; j <= yMax; j++) {
                shapePixels.push_back(QPoint(i, j));

                if (isMirrorMode) {
                    shapePixels.push_back(mirrorPixel(QPoint(i, j)));
                }
            }
        }

        for (QPoint pixel : shapePixels) emit painted(pixel, color);
    } else {
        moveAndDisplayPixels(color);
    }
}

void Canvas::circlePainting(QColor color) {

    redrawShape();

    if (isPressingMouse) {
        shapePixels.clear();

        QPoint center = shapeStartPos;
        QPoint edge = mousePixelPos;
        int radius = std::sqrt(std::pow(edge.x() - center.x(), 2) + std::pow(edge.y() - center.y(), 2));

        int x = radius;
        int y = 0;
        int decision = 1 - x;

        // Loop to draw the points on the circle's outline with optional mirroring
        while (y <= x) {
            // Draw the 8 symmetrical points on the circle
            QVector<QPoint> circlePoints = {
                QPoint(center.x() + x, center.y() + y),
                QPoint(center.x() - x, center.y() + y),
                QPoint(center.x() + x, center.y() - y),
                QPoint(center.x() - x, center.y() - y),
                QPoint(center.x() + y, center.y() + x),
                QPoint(center.x() - y, center.y() + x),
                QPoint(center.x() + y, center.y() - x),
                QPoint(center.x() - y, center.y() - x)
            };

            for (QPoint point : circlePoints) {
                shapePixels.push_back(point);

                // If mirroring is enabled, add mirrored pixels
                if (isMirrorMode) {
                    shapePixels.push_back(mirrorPixel(point));
                }
            }

            y++;

            // Update decision parameter
            if (decision <= 0) {
                decision += 2 * y + 1;
            } else {
                x--;
                decision += 2 * (y - x) + 1;
            }
        }

        for (QPoint pixel : shapePixels) emit painted(pixel, color);
    } else {
        moveAndDisplayPixels(color);
    }
}

void Canvas::circleFilledPainting(QColor color) {
    redrawShape();

    if (isPressingMouse) {
        shapePixels.clear();

        QPoint center = shapeStartPos;
        QPoint edge = mousePixelPos;
        int radius = std::sqrt(std::pow(edge.x() - center.x(), 2) + std::pow(edge.y() - center.y(), 2));

        int x = radius;
        int y = 0;
        int decision = 1 - x;

        // Loop to fill the circle
        while (y <= x) {
            // Draw horizontal lines to fill the circle
            for (int i = center.x() - x; i <= center.x() + x; i++) {
                shapePixels.push_back(QPoint(i, center.y() + y));
                shapePixels.push_back(QPoint(i, center.y() - y));

                // Mirroring pixels if isMirrorMode is enabled
                if (isMirrorMode) {
                    shapePixels.push_back(mirrorPixel(QPoint(i, center.y() + y)));
                    shapePixels.push_back(mirrorPixel(QPoint(i, center.y() - y)));
                }
            }

            for (int i = center.x() - y; i <= center.x() + y; i++) {
                shapePixels.push_back(QPoint(i, center.y() + x));
                shapePixels.push_back(QPoint(i, center.y() - x));

                // Mirroring pixels if isMirrorMode is enabled
                if (isMirrorMode) {
                    shapePixels.push_back(mirrorPixel(QPoint(i, center.y() + x)));
                    shapePixels.push_back(mirrorPixel(QPoint(i, center.y() - x)));
                }
            }

            y++;

            // Update decision parameter
            if (decision <= 0) {
                decision += 2 * y + 1;
            } else {
                x--;
                decision += 2 * (y - x) + 1;
            }
        }

        for (QPoint pixel : shapePixels) emit painted(pixel, color);
    } else {
        moveAndDisplayPixels(color);
    }
}

void Canvas::trianglePainting(QColor color) {

    redrawShape();

    if (isPressingMouse) {
        shapePixels.clear();

        // Define the three vertices of the triangle
        QPoint vertex1 = shapeStartPos; // Start position (first vertex)
        QPoint vertex2(mousePixelPos.x(), shapeStartPos.y()); // Second vertex (horizontal from start)
        QPoint vertex3((shapeStartPos.x() + mousePixelPos.x()) / 2, mousePixelPos.y()); // Third vertex (below, forming an isosceles triangle)

        // Helper lambda to draw a line between two points
        auto drawLine = [&](QPoint p1, QPoint p2) {
            int x1 = p1.x();
            int y1 = p1.y();
            int x2 = p2.x();
            int y2 = p2.y();

            int dx = abs(x2 - x1), dy = abs(y2 - y1);
            int sx = (x1 < x2) ? 1 : -1;
            int sy = (y1 < y2) ? 1 : -1;
            int err = dx - dy;

            while (true) {
                shapePixels.push_back(QPoint(x1, y1));

                if (isMirrorMode) {
                    shapePixels.push_back(mirrorPixel(QPoint(x1, y1)));
                }

                if (x1 == x2 && y1 == y2) break;
                int e2 = 2 * err;
                if (e2 > -dy) { err -= dy; x1 += sx; }
                if (e2 < dx) { err += dx; y1 += sy; }
            }
        };

        // Draw the three edges of the triangle
        drawLine(vertex1, vertex2); // Top edge
        drawLine(vertex2, vertex3); // Right edge
        drawLine(vertex3, vertex1); // Left edge

        for (QPoint pixel : shapePixels) emit painted(pixel, color);
    } else {
        moveAndDisplayPixels(color);
    }
}

void Canvas::triangleFilledPainting(QColor color) {

    redrawShape();

    if (isPressingMouse) {
        shapePixels.clear();

        // Define the three vertices of the triangle
        QPoint vertex1 = shapeStartPos; // Top vertex
        QPoint vertex2(mousePixelPos.x(), shapeStartPos.y()); // Second vertex (horizontal from start)
        QPoint vertex3((shapeStartPos.x() + mousePixelPos.x()) / 2, mousePixelPos.y()); // Bottom vertex to form an isosceles triangle

        // Sort vertices by y-coordinate to simplify scanning from top to bottom
        if (vertex2.y() < vertex1.y()) std::swap(vertex1, vertex2);
        if (vertex3.y() < vertex1.y()) std::swap(vertex1, vertex3);
        if (vertex3.y() < vertex2.y()) std::swap(vertex2, vertex3);

        // Lambda to interpolate x-coordinates for a given y between two points
        auto interpolateX = [](QPoint p1, QPoint p2, int y) -> int {
            if (p1.y() == p2.y()) return p1.x(); // Avoid division by zero
            return p1.x() + (y - p1.y()) * (p2.x() - p1.x()) / (p2.y() - p1.y());
        };

        // Fill the triangle using horizontal scan lines
        for (int y = vertex1.y(); y <= vertex3.y(); y++) {
            int xStart, xEnd;

            // Determine the start and end x-coordinates for the current scanline
            if (y < vertex2.y()) { // Top half (vertex1 to vertex2 and vertex3)
                xStart = interpolateX(vertex1, vertex3, y);
                xEnd = interpolateX(vertex1, vertex2, y);
            } else { // Bottom half (vertex2 to vertex3)
                xStart = interpolateX(vertex1, vertex3, y);
                xEnd = interpolateX(vertex2, vertex3, y);
            }

            // Ensure xStart is less than xEnd
            if (xStart > xEnd) std::swap(xStart, xEnd);

            // Push points for each x position along the scanline
            for (int x = xStart; x <= xEnd; x++) {
                shapePixels.push_back(QPoint(x, y));

                // Mirroring pixels if isMirrorMode is enabled
                if (isMirrorMode) {
                    shapePixels.push_back(mirrorPixel(QPoint(x, y)));
                }
            }
        }

        for (QPoint pixel : shapePixels) emit painted(pixel, color);
    } else {
        moveAndDisplayPixels(color);
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    QPoint localPos = event->pos();

    int col = localPos.x() / pixelSize;
    int row = localPos.y() / pixelSize;

    mousePixelPos = QPoint(col, row);

    if (isPressingMouse && mousePixelPos != QPoint(-1, -1)) paintPixels();
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    isPressingMouse = true;

    mousePixelPos = convertWorldToPixel(event->pos());

    if (isShapeMode) {
        shapeStartPos = mousePixelPos;
    }

    if (mousePixelPos != QPoint(-1, -1)) paintPixels();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    isPressingMouse = false;

    if (isShapeMode) {
        paintPixels();
    }
}

QPoint Canvas::convertWorldToPixel(QPoint mousePos) {
    int x =  mousePos.x() / pixelSize;
    int y =  mousePos.y() / pixelSize;

    return QPoint(x, y);
}

void Canvas::paintCheckerBoard(int resolution) {
    backgroundPixmap = QPixmap(resolution, resolution);
    backgroundPixmap.fill(Qt::transparent);
    QPainter painter(&backgroundPixmap);

    QColor black(117, 117, 117);
    QColor white(204, 204, 204);

    for (int row = 0; row < sideLength; ++row) {
        for (int col = 0; col < sideLength; ++col) {
            if ((row + col) % 2 == 0) {
                painter.fillRect(col * pixelSize, row * pixelSize, pixelSize, pixelSize, white);
            } else {
                painter.fillRect(col * pixelSize, row * pixelSize, pixelSize, pixelSize, black);
            }
        }
    }
}


void Canvas::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    QSize available = size();

    uiMinSide = qMin(available.width(), available.height());

    pixelSize = uiMinSide / sideLength;

    paintCheckerBoard(uiMinSide);

    if (foregroundPixmap != nullptr) {
        repaint();
    }
}
