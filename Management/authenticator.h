#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <QString>
#include "Infrastructure/DataBase/storage_manager.h"

enum class AuthResult
{
    SUCCESS,

    USER_NOT_FOUND,
    WRONG_PASSWORD,

    USERNAME_TAKEN,
    EMAIL_TAKEN,
    PHONE_TAKEN,

    INVALID_EMAIL,
    INVALID_PHONE,
    WEAK_PASSWORD,

    USER_PHONE_MISMATCH,

    UNKNOWN_ERROR
};

class Authenticator
{
public:
    Authenticator();

    AuthResult login(const QString& username, const QString& password);

    AuthResult signup(const QString& name, const QString& username, const QString& email, const QString& phone, const QString& password);

    AuthResult resetPassword(const QString& username, const QString& phone, const QString& newPassword);

    AuthResult verifyUserPhone(const QString& username, const QString& phone);

private:
    StorageManager storage;
};

#endif // AUTHENTICATOR_H
