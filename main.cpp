#include "UI/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qApp->setStyle("Fusion");
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::white);
    palette.setColor(QPalette::WindowText, Qt::black);
    qApp->setPalette(palette);

    MainWindow w;
    w.show();
    return a.exec();
}
