#include "framemanager.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

FrameManager::FrameManager(int sideLength, int pixelSize, int fps, QObject *parent)
    : selectedFrameIndex(-1), sideLength(sideLength), pixelSize(pixelSize), fps(fps), QObject{parent} {
}

void FrameManager::setSideLength(int length) {
    sideLength = length;
    for (Frame* frame : frames) {
        frame->resizePixmap(sideLength, pixelSize);
    }

    emit sideLengthChanged(sideLength);
}

void FrameManager::selectFrame(int frameIndex) {
    if (frameIndex >= 0 && frameIndex < int(frames.size())) {
        selectedFrameIndex = frameIndex;
        emit selectedFrameChanged(getSelectedFrame());
    }
}

void FrameManager::onFrameAdded() {
    qDebug() << "onFrameAdded slot exec";
    Frame* newFrame = new Frame(sideLength, pixelSize);

    // Add the newly created Frame object to the vector
    frames.push_back(newFrame);

    if (selectedFrameIndex == -1) {
        selectFrame(0); // Select the first frame by default
    }
}

void FrameManager::removeFrame(int frameIndex) {
    if (frameIndex >= 0 && frameIndex < int(frames.size())) {
        frames.erase(frames.begin() + frameIndex);
        // If the selected frame is removed, select the previous one or the first
        if (selectedFrameIndex >= int(frames.size())) {
            selectFrame(frames.size() - 1);
        }
    }
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
    qDebug() << "onPainted slots exec";
}
