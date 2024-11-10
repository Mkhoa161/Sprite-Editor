#include "mainwindow.h"
#include "framemanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FrameManager frameManager;
    MainWindow w(frameManager);

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
                             "}"
                            ;

    a.setStyleSheet(brightStyleSheet);

    w.show();
    return a.exec();
}
