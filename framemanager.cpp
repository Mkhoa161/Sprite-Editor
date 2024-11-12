#include "framemanager.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

FrameManager::FrameManager(int sideLength, int fps, QObject *parent)
    : selectedFrameIndex(-1), sideLength(sideLength), fps(fps), QObject{parent} {
    connect(&animationTimer, &QTimer::timeout, this, &FrameManager::updatePreview);
    animationTimer.start(1000 / fps);
}

void FrameManager::setSideLength(int length) {
    sideLength = length;
    for (Frame* frame : frames) {
        frame->resizePixmap(sideLength);
    }

    emit sideLengthChanged(sideLength);
}

void FrameManager::selectFrame(int frameIndex) {
    if (frameIndex >= 0 && frameIndex < int(frames.size())) {
        selectedFrameIndex = frameIndex;
        emit selectedFrameChanged(getSelectedFrame());
    }
    emit selectFrameSignal(selectedFrameIndex);
}

void FrameManager::onFrameAdded() {
    qDebug() << "onFrameAdded slot exec";
    Frame* newFrame = new Frame(sideLength);

    // Add the newly created Frame object to the vector
    frames.push_back(newFrame);

    if (selectedFrameIndex == -1) {
        selectFrame(0); // Select the first frame by default
    }

    emit frameCountChanged(frames.size());
    emit framesChanged(getFrames());

    selectFrame(frames.size() - 1);
}

void FrameManager::removeFrame(int frameIndex) {
    if (frameIndex >= 0 && frameIndex < int(frames.size())) {
        frames.erase(frames.begin() + frameIndex);
        // If the selected frame is removed, select the previous one or the first
        if (selectedFrameIndex >= int(frames.size())) {
            selectFrame(frames.size() - 1);
        }
    }
    emit frameCountChanged(frames.size());
}

void FrameManager::setFrameIndex(int frameIndex, int newIndex) {
    if (frameIndex >= 0 && frameIndex < int(frames.size()) && newIndex >= 0 && newIndex < int(frames.size())) {
        // Swap the frames
        std::swap(frames[frameIndex], frames[newIndex]);

        if (selectedFrameIndex == frameIndex) {
            selectedFrameIndex = newIndex;
        } else if (selectedFrameIndex == newIndex) {
            selectedFrameIndex = frameIndex;
        }
    }
}

// Get the currently selected frame (const reference)
Frame* FrameManager::getSelectedFrame() {
    if (selectedFrameIndex >= 0 && selectedFrameIndex < int(frames.size())) {
        return frames[selectedFrameIndex];
    }

    // Return a reference to an empty frame if none is selected (you can handle this as needed)
    return nullptr;
}

std::vector<Frame*>& FrameManager::getFrames() {
    return frames;
}

void FrameManager::onPainted(QPoint pixelPos, QColor color){
    getSelectedFrame()->updatePixmap(pixelPos, color);
    emit framesChanged(getFrames());
}

void FrameManager::onFrameSelect(int frameIndex){
    selectFrame(frameIndex);
}

void FrameManager::fpsUpdated(int newFps)
{
    fps = newFps;
    if (fps > 0) {
        animationTimer.setInterval(1000 / fps);
        animationTimer.start();
    } else {
        animationTimer.stop();
    }
}

void FrameManager::updatePreview() {
    if (!frames.empty()) {
        emit updateAnimationPreview(*frames[animFrameIndex]);
        animFrameIndex = (animFrameIndex + 1) % frames.size();
    }
}
