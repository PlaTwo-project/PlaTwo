#include <QApplication>
#include <QIcon>
#include "Management/App/app_manager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/Resources/icon.ico"));

    qApp->setStyle("Fusion");
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::white);
    palette.setColor(QPalette::WindowText, Qt::black);
    qApp->setPalette(palette);

    AppManager manager;
    manager.run();

    return app.exec();
}