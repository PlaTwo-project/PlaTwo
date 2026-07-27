#include <QApplication>
#include <QFontDatabase>
#include <QIcon>
#include "Management/App/app_manager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/Resources/Icon/icon.ico"));

    QFontDatabase::addApplicationFont(":/fonts/Resources/Fonts/Orbitron-VariableFont_wght.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Resources/Fonts/LTe50402.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Resources/Fonts/LTe50403.ttf");

    qApp->setStyle("Fusion");
    QPalette palette;
    palette.setColor(QPalette::Window, QColor("#EAD7B0"));
    palette.setColor(QPalette::WindowText, QColor("#3A2412"));
    qApp->setPalette(palette);

    AppManager manager;
    manager.run();

    return app.exec();
}