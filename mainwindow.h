#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include "canvas.h"
#include "framemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(FrameManager& frameManager, QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void toolSelected(Canvas::Mode mode);
    void setCurrentColor(int r, int g, int b, int a);
    void setCurrentColorHex(QString hex);

private slots:
    void emitColorChange();
    void emitHexChange();

private:
    Ui::MainWindow *ui;
    QButtonGroup* toolButtonGroup;
    void updateColorPreview(QColor color);
};
#endif // MAINWINDOW_H
