#include "password_hasher.h"

QString PasswordHasher::hasher(const QString& password) {
    QByteArray byteArray = password.toUtf8();
    QByteArray hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

bool PasswordHasher::verify(const QString& password, const QString& hashedPassword) {
    return hasher(password) == hashedPassword;
}