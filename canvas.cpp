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
    foregroundPixmap = QPixmap(size());
    foregroundPixmap.fill(Qt::transparent);

    paintCheckerBoard();
}

Canvas::~Canvas()
{
    delete ui;
}

void Canvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.drawPixmap(0, 0, backgroundPixmap);
    painter.drawPixmap(0, 0, foregroundPixmap);
}

void Canvas::mouseMoveEvent(QMouseEvent *event){
    QPoint localPos = event->pos();

    qDebug() << "Cursor position relative to Canvas:" << localPos;

    int col = localPos.x() / pixelSize;
    int row = localPos.y() / pixelSize;

    mousePixelPos = QPoint(col, row);

    qDebug() << "Cursor pixel position:" << mousePixelPos;

    if(isPressingMouse && mousePixelPos != QPoint(-1, -1)) {
        paint(mousePixelPos);
        repaint();
    }
}

void Canvas::mousePressEvent(QMouseEvent *event){
    isPressingMouse = true;
    // mousePixelPos = convertWorldToPixel(event->pos());

    // if(mousePixelPos != QPoint(-1, -1)){
    //     paint(mousePixelPos);
    //     repaint();
    // }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    isPressingMouse = false;
}

// void Canvas::leaveEvent(QEvent *event){
//     qDebug() << "Mouse leave canvas";
// }

//Leaving this unimplemented so that the person in charge of paint functionality can implement
void Canvas::paint(QPoint pixelPos){
    // QPainter painter(&foregroundPixmap);

    // painter.setPen(color);

    // painter.fillRect(getPixelRect(pixelPos), color);

    // qDebug() << "Rect drew";
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

    const int rows = height() / pixelSize;
    const int cols = width() / pixelSize;

    QColor black(117, 117, 117);
    QColor white(204, 204, 204);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if ((row + col) % 2 == 0) {
                painter.fillRect(col * pixelSize, row * pixelSize, pixelSize, pixelSize, white);
            } else {
                painter.fillRect(col * pixelSize, row * pixelSize, pixelSize, pixelSize, black);
            }
        }
    }
}
