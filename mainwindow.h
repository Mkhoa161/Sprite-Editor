/*
    Authors: Zhuyi Bu, Zhenzhi Liu, Justin Melore, Maxwell Rodgers, Duke Nguyen, Minh Khoa Ngo
    Github usernames: 1144761429, 0doxes0, JustinMelore, maxdotr, duke7012, Mkhoa161
    Date: November 12th, 2024
    Class: CS3505, Fall 2024
    Assignment - A8: Sprite Editor Implementation

    The MainWindow class serves as the main view for the Sprite Editor, providing the user interface
    for interacting with the FrameManager model and manipulating frames in an animation sequence.
    As the central view component, MainWindow manages user inputs, coordinates UI elements such as tool selections,
    color pickers, and frame previews, and facilitates updates in response to changes in the underlying data.
*/

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
    /// \brief Constructor for the main window, connects slots and signals to FrameManager and the Canvas.
    /// \param frameManager the frameManager model
    /// \param parent The parent of this QObject, necessary for the QT framework
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
    /// \brief Slot to capture when a user changes the dimensions of the canvas.
    void onChangeDimensionClicked();

    /// \brief Slot to capture when a user changes colors, emitting the setCurrentColor signal.
    void emitColorChange();

    /// \brief Slot to capture when a user changes the hex number of a color, emitting the setCurrentColor signal.
    void emitHexChange();

    /// \brief Slot to capture when a user clicks the add frame button, emitting the frameAdded signal.
    void on_addFrameButton_clicked();

    /// \brief Slot to capture when a user adds a frame, adjusting the frame spinbox and slider.
    /// \param newFrameCount The count of frames after an insertion of deletion happens.
    void frameCountChanged(int newFrameCount);

    /// \brief Slot to update frame previews, changing the display of preview boxes.
    /// \param frames Vector of frames holding frame data to update previews with
    void updateFramePreviews(const std::vector<Frame*>& frames);

    /// \brief Slot to update the animation preview.
    /// \param frame The frame data to update the preview window with
    void updateAnimationPreview(const Frame& frame);

    /// \brief Slot to capture when a user selects a frame, updating preview's to show selection.
    /// \param index The index of the selected frame
    void onSelectFrame(int index);

    /// \brief Slot to capture when a user changes the frames per second of the animation preview.
    /// Emits the fpsUpdated signal with the new fps.
    /// \param fps The value of the frames per second spinbox
    void on_fpsSpinBox_valueChanged(int fps);

    /// \brief Slot to capture when a user loads a .sprite project file, updating display with project information.
    void onFileLoaded();

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
