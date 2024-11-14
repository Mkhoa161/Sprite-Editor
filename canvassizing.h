/*
    Authors: Zhuyi Bu, Zhenzhi Liu, Justin Melore, Maxwell Rodgers, Duke Nguyen, Minh Khoa Ngo
    Github usernames: 1144761429, 0doxes0, JustinMelore, maxdotr, duke7012, Mkhoa161
    Date: November 10th, 2024
    Class: CS3505, Fall 2024
    Assignment - A8: Sprite Editor Implementation

    The CanvasSizing class serves as the the view of the pop-up window that let users to change the side length/dimension
    of the canvas they are painting on. This file serves to set up the fields and methods for the class.

    Code style checked by: Justin Melore
*/

#ifndef CANVASSIZING_H
#define CANVASSIZING_H

// #include "canvas.h"
#include <QDialog>

namespace Ui {
class CanvasSizing;
}

///
/// \brief The CanvasSizing class allows the user to modify the size of the Canvas widget in the UI. The
/// widget appears as a popup on the UI and size of the canvas is measured in pixels.
///
class CanvasSizing : public QDialog {
    Q_OBJECT

public:
    /// \brief Constructor for the CanvasSizing object.
    /// \param parent The parent of this QDialog, necessary for the QT framework.
    explicit CanvasSizing(QWidget *parent = nullptr);

    ~CanvasSizing();

signals:
    void applyClicked(int newSideLength);

private:
    /// \brief ui The UI element of this QDialog when running.
    Ui::CanvasSizing *ui;

private slots:
    /// \brief Slot capturing when the "apply" button is clicked in the pop-up window.
    /// Emits the applyClicked signal.
    void onApplyClicked();

    /// \brief Slot capturing when the spin box in the pop-up window changes its value.
    /// \param value The new value in the spin box.
    void onSpinBoxValueChanged(int value);
};

#endif // CANVASSIZING_H
