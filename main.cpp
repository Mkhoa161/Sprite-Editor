#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;


    QString brightStyleSheet = "QWidget {"
                             "  background-color: #EEEEEE;"
                             "  color: #1E1E1E;"
                             "}"
                             "QToolButton {"
                             "  background-color: #EEEEEE;"
                             "  border: none;"
                             "  color: #1E1E1E;"
                             "}"
                             "QToolButton:hover {"
                             "  background-color: #EEEEEE;"
                             "}"
                             "QToolButton:pressed {"
                             "  background-color: #EEEEEE;"
                             "}";

    QString darkStyleSheet = "QWidget {"
                             "  background-color: #1E1E1E;"
                             "  color: #E0E0E0;"
                             "}"
                             "QToolButton {"
                             "  background-color: #3C3C3C;"
                             "  border-radius: 5px;"
                             "  padding: 5px;"
                             "  color: #eee;"
                             "}"
                             "QToolButton:hover {"
                             "  background-color: #8C8C8C;"
                             "}"
                             "QToolButton:pressed {"
                             "  background-color: #282828;"
                             "}";

    a.setStyleSheet(brightStyleSheet);

    w.show();
    return a.exec();
}
