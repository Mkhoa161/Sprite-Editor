/*
    Authors: Zhuyi Bu, Zhenzhi Liu, Justin Melore, Maxwell Rodgers, Duke Nguyen, Minh Khoa Ngo
    Github usernames: 1144761429, 0doxes0, JustinMelore, maxdotr, duke7012, Mkhoa161
    Date: November 12th, 2024
    Class: CS3505, Fall 2024
    Assignment - A8: Sprite Editor Implementation

    The Canvas class is a custom QWidget used within the Sprite Editor application, enabling users to paint, erase,
    and manipulate pixels within a designated drawing area. As the primary editing surface in the view,
    Canvas allows for pixel specific changes, supporting various tools, shapes, and colors
    to create and modify individual frames in the animation sequence.

    Code style checked by: Duke Nguyen (duke7012)
*/

#include "mainwindow.h"
#include "framemanager.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    FrameManager frameManager;
    MainWindow window(frameManager);

    QString brightStyleSheet =  "QWidget {"
                                "   background-color: #EEEEEE;"
                                "   color: #1E1E1E;"
                                "}"

                                "QToolButton {"
                                "   background-color: #EEEEEE;"
                                "   border: none;"
                                "   color: #1E1E1E;"
                                "}"

                                "QToolButton:hover {"
                                "   background-color: #EEEEEE;"
                                "}"

                                "QToolButton:pressed {"
                                "   background-color: #EEEEEE;"
                                "}"

                                "QToolButton:disabled {"
                                "   color: #000000;"
                                "   background-color: #A0A0A0;"
                                "}";

    app.setStyleSheet(brightStyleSheet);

    window.show();
    return app.exec();
}
