#include "canvas.h"
#include "ui_canvas.h"
#include <QCursor>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <vector>

using std::vector;

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Canvas)
{
    ui->setupUi(this);

    pixelSize = DEFAULT_PIXEL_SIZE;
    selectedColor = DEFAULT_COLOR;
    currentMode = DEFAULT_MODE;
    isMirrorMode = false;

    isShapeMode = false;


    backgroundPixmap = QPixmap(size());
}

Canvas::~Canvas()
{
    delete ui;
}

void Canvas::selectTool(Mode mode)
{
    currentMode = mode;
    qDebug() << "selected mode" << mode;

    if(currentMode == Mode::CIRCLE){
        qDebug() << "Shape Mode Enabled: Circle Shape selected!";
    }
    else if(currentMode == Mode::SQUARE){
        qDebug() << "Shape Mode Enabled: Square Shape selected!";
    }
    else if(currentMode == Mode::TRIANGLE){
        qDebug() << "Shape Mode Enabled: Triangle Shape selected!";
    }

    if(currentMode == Mode::CIRCLEFILLED || currentMode == Mode::SQUAREFILLED || currentMode == Mode::TRIANGLEFILLED){
        isShapeMode = true;
        qDebug() << "Filled Shape enabled!";
    }
    else if(currentMode == Mode::CIRCLE || currentMode == Mode::SQUARE || currentMode == Mode::TRIANGLE){
        isShapeMode = true;
        qDebug() << "Filled Shape disabled!";
    }
    else{
        isShapeMode = false;
    }
}

void Canvas::setMirrorMode(bool enabled) {
    isMirrorMode = enabled;
    qDebug() << "Mirror Mode Enabled: " << isMirrorMode;
}

void Canvas::setCurrentColor(int r, int g, int b, int a){
    selectedColor = QColor(r, g, b, a);
    //qDebug() << "Color changed. r: " << r << "g: " << g << "b: " << b << "a: " << a;
}

void Canvas::onSelectedFrameChanged(Frame *newSelectedFrame){
    foregroundPixmap = &(newSelectedFrame->pixmap);
    repaint();
}

void Canvas::onSideLengthChanged(int newSideLength){
    sideLength = newSideLength;
    pixelSize = width() / sideLength;

    paintCheckerBoard();
}

void Canvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.drawPixmap(0, 0, backgroundPixmap);

    if(foregroundPixmap != nullptr){
        int scaledResolution = foregroundPixmap->height() * pixelSize;
        painter.drawPixmap(0, 0, foregroundPixmap->scaled(scaledResolution, scaledResolution));
    }
}

void Canvas::paintPixels() {
    QColor color;
    if(currentMode == Mode::ERASER) {
        color = Qt::transparent;
    }
    else {
        color = selectedColor;
    }

    if(currentMode == Mode::BRUSH){
        //This will be changed once paint is expanded to include shape tools
        paintedPixels.push_back(QPoint(mousePixelPos));
        paintedColors.push_back(color);

        if(isMirrorMode) {
            int newPosition = width() / pixelSize - mousePixelPos.x();

            QPoint mirroredPixel = QPoint(newPosition, mousePixelPos.y());
            paintedPixels.push_back(mirroredPixel);
            paintedColors.push_back(color);

            emit paint(mirroredPixel, color);
        }

        emit paint(mousePixelPos, color);
    }

    if(currentMode == Mode::ERASER){
        vector<QPoint> newPaintedPixels;
        vector<QColor> newPaintedColors;

        for (int i = 0; i < int(paintedPixels.size()); i++) {
            QPoint currPixel = paintedPixels[i];
            QColor currColor = paintedColors[i];
            if (!(currPixel.x() == mousePixelPos.x() && currPixel.y() == mousePixelPos.y())) {
                if (isMirrorMode) {
                    QPoint mirroredPixel = QPoint(width() / pixelSize - currPixel.x(), currPixel.y());
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

        for (int i = 0; i < int(newPaintedPixels.size()); i++){
            paintedPixels.push_back(newPaintedPixels[i]);
            paintedColors.push_back(newPaintedColors[i]);
        }

        emit paint(QPoint(mousePixelPos), color);

        if(isMirrorMode){
            QPoint mirroredPixel = QPoint(width() / pixelSize - mousePixelPos.x(), mousePixelPos.y());
            emit paint(mirroredPixel, color);
        }
    }

    if(currentMode == Mode::SQUARE){
        for(QPoint shapePixel : shapePixels) {
            emit paint(shapePixel, Qt::transparent);
        }

        for (int i = 0; i < int(paintedPixels.size()); i++){
            QPoint currPixel = paintedPixels[i];
            QColor currColor = paintedColors[i];
            emit paint(currPixel, currColor);
        }

        if(isPressingMouse){
            shapePixels.clear();

            QPoint startPixel = shapeStartPos;
            QPoint endPixel = mousePixelPos;

            int x1 = std::min(startPixel.x(), endPixel.x());
            int y1 = std::min(startPixel.y(), endPixel.y());
            int x2 = std::max(startPixel.x(), endPixel.x());
            int y2 = std::max(startPixel.y(), endPixel.y());

            // Top and bottom edges
            for(int i = x1; i <= x2; ++i){
                shapePixels.push_back(QPoint(i, y1)); // Top edge
                shapePixels.push_back(QPoint(i, y2)); // Bottom edge

                if(isMirrorMode) {
                    int newPosition = width() / pixelSize - i;

                    QPoint mirroredPixel1 = QPoint(newPosition, y1);
                    shapePixels.push_back(mirroredPixel1);

                    QPoint mirroredPixel2 = QPoint(newPosition, y2);
                    shapePixels.push_back(mirroredPixel2);
                }
            }

            // Left and right edges
            for(int j = y1 + 1; j < y2; ++j){
                shapePixels.push_back(QPoint(x1, j)); // Left edge
                shapePixels.push_back(QPoint(x2, j)); // Right edge

                if(isMirrorMode) {
                    int newPosition1 = width() / pixelSize - x1;

                    QPoint mirroredPixel1 = QPoint(newPosition1, j);
                    shapePixels.push_back(mirroredPixel1);

                    int newPosition2 = width() / pixelSize - x2;
                    QPoint mirroredPixel2 = QPoint(newPosition2, j);
                    shapePixels.push_back(mirroredPixel2);
                }
            }

            for(QPoint pixel : shapePixels) emit paint(pixel, color);
        }
        else{
            for (int i = 0; i < int(paintedPixels.size()); i++){
                QPoint currPixel = paintedPixels[i];
                QColor currColor = paintedColors[i];
                emit paint(currPixel, currColor);
            }

            for(QPoint pixel : shapePixels) {
                paintedPixels.push_back(pixel);
                paintedColors.push_back(color);
                emit paint(pixel, color);
            }
            shapePixels.clear();
        }
    }

    if(currentMode == Mode::SQUAREFILLED){
        for(QPoint shapePixel : shapePixels) {
            emit paint(shapePixel, Qt::transparent);
        }

        //for(QPoint pixel : paintedPixels) emit paint(pixel, color);
        for (int i = 0; i < int(paintedPixels.size()); i++){
            QPoint currPixel = paintedPixels[i];
            QColor currColor = paintedColors[i];
            emit paint(currPixel, currColor);
        }

        if(isPressingMouse){
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

                    if(isMirrorMode) {
                        int newPosition = width() / pixelSize - i;

                        QPoint mirroredPixel = QPoint(newPosition, j);
                        shapePixels.push_back(mirroredPixel);
                    }
                }
            }

            for(QPoint pixel : shapePixels) emit paint(pixel, color);
        }
        else{
            for (int i = 0; i < int(paintedPixels.size()); i++){
                QPoint currPixel = paintedPixels[i];
                QColor currColor = paintedColors[i];
                emit paint(currPixel, currColor);
            }

            for(QPoint pixel : shapePixels) {
                paintedPixels.push_back(pixel);
                paintedColors.push_back(color);
                emit paint(pixel, color);
            }
            shapePixels.clear();
        }
    }

    if(currentMode == Mode::CIRCLE){
        for(QPoint shapePixel : shapePixels) {
            emit paint(shapePixel, Qt::transparent);
        }

        //for(QPoint pixel : paintedPixels) emit paint(pixel, color);
        for (int i = 0; i < int(paintedPixels.size()); i++){
            QPoint currPixel = paintedPixels[i];
            QColor currColor = paintedColors[i];
            emit paint(currPixel, currColor);
        }

        if(isPressingMouse){
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
                        int mirroredX = width() / pixelSize - point.x();
                        QPoint mirroredPixel(mirroredX, point.y());
                        shapePixels.push_back(mirroredPixel);
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

            for(QPoint pixel : shapePixels) emit paint(pixel, color);
        }
        else{
            for (int i = 0; i < int(paintedPixels.size()); i++){
                QPoint currPixel = paintedPixels[i];
                QColor currColor = paintedColors[i];
                emit paint(currPixel, currColor);
            }

            for(QPoint pixel : shapePixels) {
                paintedPixels.push_back(pixel);
                paintedColors.push_back(color);
                emit paint(pixel, color);
            }
            shapePixels.clear();
        }
    }

    if(currentMode == Mode::CIRCLEFILLED){
        for(QPoint shapePixel : shapePixels) {
            emit paint(shapePixel, Qt::transparent);
        }

        //for(QPoint pixel : paintedPixels) emit paint(pixel, color);
        for (int i = 0; i < int(paintedPixels.size()); i++){
            QPoint currPixel = paintedPixels[i];
            QColor currColor = paintedColors[i];
            emit paint(currPixel, currColor);
        }

        if(isPressingMouse){
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
                        int mirroredX = width() / pixelSize - i;
                        shapePixels.push_back(QPoint(mirroredX, center.y() + y));
                        shapePixels.push_back(QPoint(mirroredX, center.y() - y));
                    }
                }

                for (int i = center.x() - y; i <= center.x() + y; i++) {
                    shapePixels.push_back(QPoint(i, center.y() + x));
                    shapePixels.push_back(QPoint(i, center.y() - x));

                    // Mirroring pixels if isMirrorMode is enabled
                    if (isMirrorMode) {
                        int mirroredX = width() / pixelSize - i;
                        shapePixels.push_back(QPoint(mirroredX, center.y() + x));
                        shapePixels.push_back(QPoint(mirroredX, center.y() - x));
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

            for(QPoint pixel : shapePixels) emit paint(pixel, color);
        }
        else{
            for (int i = 0; i < int(paintedPixels.size()); i++){
                QPoint currPixel = paintedPixels[i];
                QColor currColor = paintedColors[i];
                emit paint(currPixel, currColor);
            }

            for(QPoint pixel : shapePixels) {
                paintedPixels.push_back(pixel);
                paintedColors.push_back(color);
                emit paint(pixel, color);
            }
            shapePixels.clear();
        }
    }

    if(currentMode == Mode::TRIANGLE){
        for(QPoint shapePixel : shapePixels) {
            emit paint(shapePixel, Qt::transparent);
        }

        //for(QPoint pixel : paintedPixels) emit paint(pixel, color);
        for (int i = 0; i < int(paintedPixels.size()); i++){
            QPoint currPixel = paintedPixels[i];
            QColor currColor = paintedColors[i];
            emit paint(currPixel, currColor);
        }

        if(isPressingMouse){
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
                        int mirroredX = width() / pixelSize - x1;
                        shapePixels.push_back(QPoint(mirroredX, y1));
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

            for(QPoint pixel : shapePixels) emit paint(pixel, color);
        }
        else{
            for (int i = 0; i < int(paintedPixels.size()); i++){
                QPoint currPixel = paintedPixels[i];
                QColor currColor = paintedColors[i];
                emit paint(currPixel, currColor);
            }

            for(QPoint pixel : shapePixels) {
                paintedPixels.push_back(pixel);
                paintedColors.push_back(color);
                emit paint(pixel, color);
            }
            shapePixels.clear();
        }
    }

    if(currentMode == Mode::TRIANGLEFILLED){
        for(QPoint shapePixel : shapePixels) {
            emit paint(shapePixel, Qt::transparent);
        }

        //for(QPoint pixel : paintedPixels) emit paint(pixel, color);
        for (int i = 0; i < int(paintedPixels.size()); i++){
            QPoint currPixel = paintedPixels[i];
            QColor currColor = paintedColors[i];
            emit paint(currPixel, currColor);
        }

        if(isPressingMouse){
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
                        int mirroredX = width() / pixelSize - x;
                        shapePixels.push_back(QPoint(mirroredX, y));
                    }
                }
            }

            for(QPoint pixel : shapePixels) emit paint(pixel, color);
        }
        else{
            for (int i = 0; i < int(paintedPixels.size()); i++){
                QPoint currPixel = paintedPixels[i];
                QColor currColor = paintedColors[i];
                emit paint(currPixel, currColor);
            }

            for(QPoint pixel : shapePixels) {
                paintedPixels.push_back(pixel);
                paintedColors.push_back(color);
                emit paint(pixel, color);
            }
            shapePixels.clear();
        }
    }

    repaint();
}

void Canvas::mouseMoveEvent(QMouseEvent *event){
    QPoint localPos = event->pos();

    int col = localPos.x() / pixelSize;
    int row = localPos.y() / pixelSize;

    mousePixelPos = QPoint(col, row);

    if(isPressingMouse && mousePixelPos != QPoint(-1, -1)) paintPixels();
}

void Canvas::mousePressEvent(QMouseEvent *event){
    isPressingMouse = true;

    mousePixelPos = convertWorldToPixel(event->pos());

    if(isShapeMode){
        shapeStartPos = mousePixelPos;
    }

    if(mousePixelPos != QPoint(-1, -1)) paintPixels();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    isPressingMouse = false;

    if(isShapeMode){
        paintPixels();
        paintedPixels.clear();
        paintedColors.clear();
    }
}

QRect Canvas::getPixelRect(QPoint pixelPos){
    return QRect(pixelPos * pixelSize, QSize(pixelSize, pixelSize));
}

QPoint Canvas::convertWorldToPixel(QPoint mousePos){
    int x =  mousePos.x() / pixelSize;
    int y =  mousePos.y() / pixelSize;

    return QPoint(x, y);
}

void Canvas::paintCheckerBoard(){
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
