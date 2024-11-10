#ifndef FRAMEMANAGER_H
#define FRAMEMANAGER_H

#include "frame.h"
#include <vector>

class FrameManager
{
public:
    FrameManager(int sideLength = 100, int pixelSize = 10, int fps = 30);
    void setSideLength(int length);
    void selectFrame(int frameIndex);
    void addFrame();
    void removeFrame(int frameIndex);
    void setFrameIndex(int frameIndex, int newIndex);
    const Frame& getSelectedFrame() const;
    const std::vector<Frame>& getFrames() const;

public slots:
    void paintCurrentFrame(QPoint pixelPos, QColor color);

private:
    std::vector<Frame> frames;
    int selectedFrameIndex;
    int sideLength;
    int pixelSize;
    int fps;
};

#endif // FRAMEMANAGER_H
