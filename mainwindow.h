#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include "canvas.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void toolSelected(Canvas::Mode mode);

private slots:
    void on_penButton_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QButtonGroup* toolButtonGroup;
};
#endif // MAINWINDOW_H
