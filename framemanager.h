/*
    Authors: Zhuyi Bu, Zhenzhi Liu, Justin Melore, Maxwell Rodgers, Duke Nguyen, Minh Khoa Ngo
    Github usernames: 1144761429, 0doxes0, JustinMelore, maxdotr, duke7012, Mkhoa161
    Date: November 12th, 2024
    Class: CS3505, Fall 2024
    Assignment - A8: Sprite Editor Implementation

    The FrameManager class is the core model for the Sprite Editor, managing all frames in an animation sequence.
    It maintains and manipulates a collection of Frame objects, enabling frame selection, addition, removal, and ordering.
    This class also coordinates animation playback, frame transformation, and updates for previewing
    animations, all while ensuring changes are communicated to connected views and controllers.

    Reviewed by Zhenzhi Liu
*/

#ifndef FRAMEMANAGER_H
#define FRAMEMANAGER_H

#include <QObject>
#include <QPoint>
#include <QColor>
#include <QTimer>
#include <QString>
#include <QJsonDocument>
#include <vector>
#include "frame.h"

class FrameManager : public QObject
{
    Q_OBJECT
public:
    /// \brief Constructor for the frame manager
    /// \param sidelength The sidelength of each frame, in terms of pixels
    /// \param fps The frames per second the animation preview should default to
    /// \param parent The parent of this QObject, necessary for the QT framework
    explicit FrameManager(int sideLength = 16, int fps = 30, QObject *parent = nullptr);

    /// \brief Select a frame stored in the frame manager. Emits the selectFrameSignal to listeners to reflect the selection.
    /// \param frameIndex The index of the frame to select. Must be a valid index within the the frames vector.
    void selectFrame(int frameIndex);

    /// \brief Removes a frame stored in the frame manager, selecting the closest valid frame.
    /// Must be at least two frames stored for any frames to be removed.
    /// \param frameIndex The index of the frame to remove.
    void removeFrame(int frameIndex);

    /// \brief Changes the index of a frame, and reorders stored frames to reflect this change.
    /// \param frame frameIndex The index of the frame that will be updated.
    /// \param newIndex The new index of the frame
    void setFrameIndex(int frameIndex, int newIndex);

    /// \brief Returns a Frame object of the currently selected frame.
    Frame* getSelectedFrame();

    /// \brief Returns a vector of Frame objects stored in the frame manager.
    std::vector<Frame*>& getFrames();

signals:
    void selectedFrameChanged(Frame* newSelectedFrame);
    void sideLengthChanged(int newSideLength);
    void framesChanged(const std::vector<Frame*>& frames);
    void frameCountChanged(int newCount);
    void frameSelected(int frameIndex);
    void animationPreviewUpdated(const Frame& frame);
    void fileLoaded();

public slots:
    /// \brief Slot capturing when a frame is painted and updating the stored pixmap to reflect this change.
    /// \param pixelPos The position of the pixel that was painted.
    /// \param color The color of the painted pixel.
    void onPainted(QPoint pixelPos, QColor color);

    /// \brief Slot capturing when a frame is selected by a user.
    /// \param frameIndex the index of the selected frame.
    void onFrameSelect(int frameIndex);

    /// \brief Slot capturing when a frame is added by a user.
    void onFrameAdded();

    /// \brief Slot capturing when a frame is removed by a user.
    void onFrameRemove();

    /// \brief Slot capturing when a user changes the side length of the canvas.
    /// \param length The new length of frames.
    void onSetSideLength(int length);

    /// \brief Slot capturing when a user updates the frames per second of the animation preview.
    /// \param newFps The new frames per second of the animation preview.
    void onFpsUpdated(int newFps);

    /// \brief Slot to update the animation preview. Emits the updateAnimationPreview signal, providing a reference to the frame.
    /// Called by a QTimer, with the interval declared by the current frames per second stored in the Frame Manager.
    void onUpdatePreview();

    /// \brief Slot capturing when the user rotates frames clockwise.
    /// Emits the selectedFrameChanged and the framesChanged signals.
    void onRotateCW();

    /// \brief Slot capturing when the user rotates frames counter clockwise.
    /// Emits the selectedFrameChanged and the framesChanged signals.
    void onRotateCCW();

    /// \brief Slot capturing when the user flips frames along the X axis.
    /// Emits the selectedFrameChanged and the framesChanged signals.
    void onFlipAlongX();

    /// \brief Slot capturing when the user flips frames along the Y axis.
    /// Emits the selectedFrameChanged and the framesChanged signals.
    void onFlipAlongY();

    /// \brief Slot capturing when the user saves their project, saving all frames stored in the frame manager to a serializable format.
    /// Files are saved with the file extension .sprite and can be loaded to continue working on a project starting from where the project
    /// was saved.
    void onSaveFile();

    /// \brief Slot capturing when the user loads a project.
    /// Users may load files with the format .sprite to initialize the sprite editor with a previously saved project.
    void onLoadFile();
    
private:
    int selectedFrameIndex;
    int sideLength;
    int fps;
    int animFrameIndex = 0;
    std::vector<Frame*> frames;
    QTimer animationTimer;
};

#endif // FRAMEMANAGER_H
