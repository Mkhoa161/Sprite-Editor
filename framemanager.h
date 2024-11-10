#ifndef FRAMEMANAGER_H
#define FRAMEMANAGER_H

#include <QObject>
#include <QPoint>
#include <QColor>
#include <vector>
#include "frame.h"

class FrameManager : public QObject
{
    Q_OBJECT
public:
    explicit FrameManager(int sideLength = 16, int fps = 30, QObject *parent = nullptr);
    void setSideLength(int length);
    void selectFrame(int frameIndex);

    void removeFrame(int frameIndex);
    void setFrameIndex(int frameIndex, int newIndex);
    Frame* getSelectedFrame();
    std::vector<Frame*>& getFrames();

signals:
    void selectedFrameChanged(Frame* newSelectedFrame);
    void sideLengthChanged(int newSideLength);

public slots:
    void onPainted(QPoint pixelPos, QColor color);
    void onFrameAdded();

private:
    std::vector<Frame*> frames;
    int selectedFrameIndex;
    int sideLength;
    int fps;
};

#endif // FRAMEMANAGER_H
