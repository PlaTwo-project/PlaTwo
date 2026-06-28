#ifndef PASSWORDHASHER_H
#define PASSWORDHASHER_H
#include <QString>
#include <QByteArray>
#include <QCryptographicHash>
using namespace std;

class PasswordHasher
{
public:
    static QString hasher(const QString& password);
    static bool verify(const QString& password, const QString& hashedPassword);
};

#endif // PASSWORDHASHER_H