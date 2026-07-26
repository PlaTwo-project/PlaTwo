#include "base_exception.h"
#include <QMessageBox>
#include <string>

BaseException::BaseException(const QString& msg, const QString& title): message(msg), title(title) {}

const char* BaseException::what() const noexcept {
    static std::string stdMsg;
    stdMsg = message.toStdString();
    return stdMsg.c_str();
}

void BaseException::showDialog(QWidget *parent) const {
    QMessageBox::critical(parent, title, message);
}
