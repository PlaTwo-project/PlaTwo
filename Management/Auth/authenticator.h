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

    WRONG_OLD_PASSWORD,
    EMPTY_FIELD,

    UNKNOWN_ERROR
};

class Authenticator
{
public:
    Authenticator();

    AuthResult login(const QString &username, const QString &password, User &logged_in_user);
    AuthResult signup(const QString &name, const QString &username, const QString &email, const QString &phone, const QString &password);
    AuthResult resetPassword(const QString &username, const QString &phone, const QString &new_password);
    AuthResult verifyUserPhone(const QString &username, const QString &phone);
    AuthResult updateUser(int id, const QString &name, const QString &username, const QString &email, const QString &phone, const QString &old_password, const QString &new_password);

private:
    StorageManager storage;
};

#endif // AUTHENTICATOR_H
