#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include "Logic/Interface/user_interface.h"
#include "Infrastructure/DataBase/session_storage_manager.h"

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
    INVALID_IP,
    WEAK_PASSWORD,

    USER_PHONE_MISMATCH,

    WRONG_OLD_PASSWORD,
    EMPTY_FIELD,

    UNKNOWN_ERROR,

    ALREADY_LOGGED_IN
};

class Authenticator
{
public:
    explicit Authenticator(UserInterface& storage);

    AuthResult login(const QString &username, const QString &password, User &logged_in_user);
    void logout(int user_id);
    AuthResult signup(const QString &name, const QString &username, const QString &email, const QString &phone, const QString &password);
    AuthResult resetPassword(const QString &username, const QString &phone, const QString &new_password);
    AuthResult verifyUserPhone(const QString &username, const QString &phone);
    AuthResult updateUser(const int id, const QString &name, const QString &username, const QString &email, const QString &phone, const QString &old_password, const QString &new_password);
    AuthResult verifyIP(const QString& ip);

private:
    UserInterface& storage;
    SessionStorageManager session_storage;
};

#endif // AUTHENTICATOR_H
