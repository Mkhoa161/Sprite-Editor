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
}

void Canvas::setMirrorMode(bool enabled) {
    isMirrorMode = enabled;
    qDebug() << "Mirror Mode Enabled: " << isMirrorMode;
}

void Canvas::setCurrentColor(int r, int g, int b, int a)
{
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
    vector<QPoint> paintedPixels;
    //This will be changed once paint is expanded to include shape tools
    paintedPixels.push_back(QPoint(mousePixelPos));

    for(QPoint pixel : paintedPixels) emit paint(pixel, color);

    if(isMirrorMode) {
        vector<QPoint> mirroredPixels = mirrorPixels(paintedPixels);
        for(QPoint pixel : mirroredPixels) emit paint(pixel, color);
    }

    repaint();
}

vector<QPoint> Canvas::mirrorPixels(vector<QPoint> pixelPositions) {
    int newPosition;
    vector<QPoint> mirroredPixels;
    for(QPoint pixel : pixelPositions) {
        newPosition = width() / pixelSize - pixel.x();
        mirroredPixels.push_back(QPoint(newPosition, pixel.y()));
    }
    return mirroredPixels;
}

void Canvas::mouseMoveEvent(QMouseEvent *event){
    QPoint localPos = event->pos();

    int col = localPos.x() / pixelSize;
    int row = localPos.y() / pixelSize;

    mousePixelPos = QPoint(col, row);

    if(isPressingMouse && mousePixelPos != QPoint(-1, -1)) paintPixels();
}

void Canvas::mousePressEvent(QMouseEvent *event){
    qDebug() << currentMode;
    isPressingMouse = true;
    mousePixelPos = convertWorldToPixel(event->pos());

    if(mousePixelPos != QPoint(-1, -1)) paintPixels();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    isPressingMouse = false;
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
    backgroundPixmap = QPixmap(size());
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
