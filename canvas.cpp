#include "canvas.h"
#include "ui_canvas.h"
#include <QCursor>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

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

void Canvas::setCurrentColor(int r, int g, int b, int a)
{
    selectedColor = QColor(r, g, b, a);
    //qDebug() << "Color changed. r: " << r << "g: " << g << "b: " << b << "a: " << a;
}

void Canvas::onSelectedFrameChanged(Frame *newSelectedFrame){
    foregroundPixmap = &(newSelectedFrame->pixmap);

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

//    painter.drawPixmap(0, 0, pixmap.scaled(newResolution, newResolution, Qt::IgnoreAspectRatio));

void Canvas::mouseMoveEvent(QMouseEvent *event){
    QPoint localPos = event->pos();

    //qDebug() << "Cursor position relative to Canvas:" << localPos;

    int col = localPos.x() / pixelSize;
    int row = localPos.y() / pixelSize;

    mousePixelPos = QPoint(col, row);

    //qDebug() << "Cursor pixel position:" << mousePixelPos;

    if(isPressingMouse && mousePixelPos != QPoint(-1, -1)) {
        emit paint(mousePixelPos, selectedColor);
        repaint();
    }
}

void Canvas::mousePressEvent(QMouseEvent *event){
    isPressingMouse = true;
    mousePixelPos = convertWorldToPixel(event->pos());

    if(mousePixelPos != QPoint(-1, -1)){
        emit paint(mousePixelPos, Qt::red);
        qDebug() << "Painted at pixel: " << mousePixelPos << "with" << selectedColor;
        repaint();
    }
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
