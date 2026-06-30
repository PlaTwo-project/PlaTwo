#include "authenticator.h"
#include "Infrastructure/DataBase/storage_manager.h"
#include "Logic/Security/validator.h"
#include "Logic/Security/password_hasher.h"

Authenticator::Authenticator()
    : storage("users.json")
{
}

AuthResult Authenticator::login(const QString& username, const QString& password)
{
    User user;

    if (!storage.getUserByUsername(username, user))
        return AuthResult::USER_NOT_FOUND;

    if (!PasswordHasher::verify(password, user.getHashedPassword()))
        return AuthResult::WRONG_PASSWORD;

    return AuthResult::SUCCESS;
}

AuthResult Authenticator::signup(const QString& name, const QString& username, const QString& email, const QString& phone, const QString& password)
{
    if (!Validator::validateEmail(email.toStdString()))
        return AuthResult::INVALID_EMAIL;

    if (!Validator::validatePhone(phone.toStdString()))
        return AuthResult::INVALID_PHONE;

    if (!Validator::validatePassword(password.toStdString()))
        return AuthResult::WEAK_PASSWORD;

    if (storage.isUsernameTaken(username))
        return AuthResult::USERNAME_TAKEN;

    if (storage.isEmailTaken(email))
        return AuthResult::EMAIL_TAKEN;

    if (storage.isPhoneNumberTaken(phone))
        return AuthResult::PHONE_TAKEN;

    QString hashedPassword = PasswordHasher::hasher(password);

    User newUser(name, username, phone, email, hashedPassword);

    if (!storage.addUser(newUser))
        return AuthResult::UNKNOWN_ERROR;

    return AuthResult::SUCCESS;
}

AuthResult Authenticator::resetPassword(const QString& username, const QString& phone, const QString& newPassword)
{
    User user;

    if (!storage.getUserByUsername(username, user))
        return AuthResult::USER_NOT_FOUND;

    if (user.getPhoneNumber() != phone)
        return AuthResult::USER_PHONE_MISMATCH;

    if (!Validator::validatePassword(newPassword.toStdString()))
        return AuthResult::WEAK_PASSWORD;

    QString hashedPassword = PasswordHasher::hasher(newPassword);

    user.setHashedPassword(hashedPassword);

    if (!storage.updateUser(user))
        return AuthResult::UNKNOWN_ERROR;

    return AuthResult::SUCCESS;
}

AuthResult Authenticator::verifyUserPhone(const QString& username, const QString& phone)
{
    User user;

    if (!storage.getUserByUsername(username, user))
        return AuthResult::USER_NOT_FOUND;

    if (user.getPhoneNumber() != phone)
        return AuthResult::USER_PHONE_MISMATCH;

    return AuthResult::SUCCESS;
}