#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QLabel>
#include "canvas.h"
#include "framemanager.h"
#include "canvassizing.h"

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
    void frameAdded();
    void frameSelect(int frameIndex);
    void fpsUpdated(int fps);

private slots:
    void emitColorChange();
    void emitHexChange();
    void on_addFrameButton_clicked();
    void frameCountChanged(int newFrameCount);
    void updateFramePreviews(const std::vector<Frame*>& frames);
    void updateAnimationPreview(const Frame& frame);
    void onSelectFrame(int index);
    void on_fpsSpinBox_valueChanged(int fps);

private:
    Ui::MainWindow *ui;
    // set to allow exclusive selection between those tools
    QButtonGroup* toolButtonGroup;
    CanvasSizing* canvasSizing;

    // used to keep track of which frame is selected and has a "frame" that we should make invisible later
    int selectedFrameIndex = -1;
    // Lables that are inside frame previews
    QList<QLabel*> frameLabels;

    // \brief A click selector that adds to frames
    bool eventFilter(QObject *obj, QEvent *event) override;
    
    void updateColorPreview(QColor color);
};
#endif // MAINWINDOW_H
