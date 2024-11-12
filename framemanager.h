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
    explicit FrameManager(int sideLength = 16, int fps = 30, QObject *parent = nullptr);

    void selectFrame(int frameIndex);

    void removeFrame(int frameIndex);
    void setFrameIndex(int frameIndex, int newIndex);
    Frame* getSelectedFrame();
    std::vector<Frame*>& getFrames();

signals:
    void selectedFrameChanged(Frame* newSelectedFrame);
    void sideLengthChanged(int newSideLength);
    void framesChanged(const std::vector<Frame*>& frames);
    void frameCountChanged(int newCount);
    void selectFrameSignal(int frameIndex);
    void updateAnimationPreview(const Frame& frame);
    void fileLoaded();

public slots:
    void onPainted(QPoint pixelPos, QColor color);
    void onFrameSelect(int frameIndex);
    void onFrameAdded();
    void onFrameRemove();
    void onSetSideLength(int length);
    void fpsUpdated(int newFps);
    void updatePreview();
    void onRotateCW();
    void onRotateCCW();
    void onFlipAlongX();
    void onFlipAlongY();
    void onSaveFile();
    void onLoadFile();
    
private:
    std::vector<Frame*> frames;
    int selectedFrameIndex;
    int sideLength;
    int fps;
    int animFrameIndex = 0;
    QTimer animationTimer;
};

#endif // FRAMEMANAGER_H
