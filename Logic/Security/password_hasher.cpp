#include "password_hasher.h"

static QString hasher(const QString& password) {
    QByteArray byteArray = password.toUtf8();
    QByteArray hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

static bool verify(const QString& password, const QString& hashedPassword){
    if(hasher(password) == hashedPassword)
        return true;
    return false;
}