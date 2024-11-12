#ifndef FRAME_H
#define FRAME_H

#include <QPixmap>
#include <QJsonArray>
#include <QPoint>
#include <QColor>

class Frame
{
public:
    ///
    /// \brief Frame Create a Frame where the pixmaps of background and foregound are empty.
    /// \param sideLength The side length of the pixmaps.
    ///
    Frame(int sideLength);

    ///
    /// \brief Frame Create a Frame by deep-copy from another Frame.
    /// \param other The other Frame to copy from.
    ///
    Frame(const Frame &other);

    ///
    /// \brief operator = Assign a Frame to this frame by doing a deep-copy.
    /// \param other The other frame to copy from
    /// \return A deep-copy of the other Frame.
    ///
    Frame &operator=(Frame other);

    ///
    /// \brief ConvertToJson Convert the foregroundPixmap of the frame into Json
    /// which stores each pixel data of this frame.
    /// \return A QJsonObject that holds the frame's Json data.
    ///
    QJsonObject ConvertToJson();

    ///
    /// \brief LoadFromJson Using data from a QJsonObject to override the foregroundPixmap.
    /// \param json A QJsonObject from where the data will be used to override.
    ///
    void LoadFromJson(QJsonObject json);

    ///
    /// \brief resizePixmap Resize the pixmap according to the new side length of pixel size.
    /// \param newSideLength The new amount of canvas pixels on each axis.
    ///
    void resizePixmap(int newSideLength);

    ///
    /// \brief updatePixmap Update the color of the pixmap at a specified canvas pixel position.
    /// \param pixelPos The canvas pixel position where the color will be updated.
    /// \param color The new color to have at pixelPos
    ///
    void updatePixmap(QPoint pixelPos, QColor color);

    ///
    ///
    ///
    ///
    void rotate(bool isClockwise);

    ///
    ///
    ///
    ///
    void flip(bool isAlongXAxis);

    ///
    /// \brief pixmap The QPixmap where the painting is stored. This will be what the user paints.
    ///
    QPixmap pixmap;

private:
    ///
    /// \brief sideLength The amount of canvas pixel on each axis.
    ///
    int sideLength;
};

#endif // FRAME_H
