#include "framemanager.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFileDialog>
#include <QIODevice>
#include <QTextStream>
#include <QByteArray>

FrameManager::FrameManager(int sideLength, int fps, QObject *parent)
    : selectedFrameIndex(-1), sideLength(sideLength), fps(fps), QObject{parent} {
    connect(&animationTimer, &QTimer::timeout, this, &FrameManager::updatePreview);
    animationTimer.start(1000 / fps);
}

void FrameManager::onSetSideLength(int length) {
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

void FrameManager::onSaveFile(){
    QString filePath = QFileDialog::getSaveFileName(
        nullptr,
        "Save File",
        QDir::homePath(),
        "Sprite Files (*.sprite);;All Files (*)");


    QFile file(filePath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream out(&file);

    QJsonArray framesJsonArray;
    for(Frame* frame : frames){
        framesJsonArray.append(frame->convertToJson());
    }

    QJsonObject finalJson;
    finalJson["sideLength"] = sideLength;
    finalJson["frames"] = framesJsonArray;

    out << QJsonDocument(finalJson).toJson(QJsonDocument::Indented);

    file.close();
}

void FrameManager::onLoadFile(){
    QString filePath = QFileDialog::getOpenFileName(
        nullptr,
        "Open File",
        QDir::homePath(),
        "Sprite Files (*.sprite)");

    if(filePath.isEmpty()){
        return;
    }

    QFile file(filePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray fileData = file.readAll();

    file.close();

    for(int i = frames.size(); i >= 0; i--){
        removeFrame(i);
    }

    //if()

    QJsonObject jsonObj = QJsonDocument::fromJson(fileData).object();

    int importedSideLength = jsonObj["sideLength"].toInt();
    if(sideLength != importedSideLength){
        onSetSideLength(importedSideLength);
    }

    for(QJsonValue value : jsonObj["frames"].toArray()){
        onFrameAdded();
        frames.back()->loadFromJson(value);
    }

    emit framesChanged(getFrames());
    emit fileLoaded();
}
