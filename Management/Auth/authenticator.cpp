#include "authenticator.h"
#include "Logic/Security/validator.h"
#include "Logic/Security/password_hasher.h"
#include "Management/App/session_manager.h"

Authenticator::Authenticator()
    : storage("users.json")
{
}

AuthResult Authenticator::login(const QString& username, const QString& password, User& logged_in_user)
{
    User user;

    if (!storage.getUserByUsername(username, user))
        return AuthResult::USER_NOT_FOUND;

    if (!PasswordHasher::verify(password, user.getHashedPassword()))
        return AuthResult::WRONG_PASSWORD;

    logged_in_user = user;
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

AuthResult Authenticator::updateUser(int id, const QString& name, const QString& username, const QString& email, const QString& phone, const QString& old_password, const QString& new_password)
{
    User user;

    if (!username.isEmpty()) {
        if (!storage.getUserById(id, user))
            return AuthResult::USER_NOT_FOUND;

        if (user.getUsername() != username && storage.isUsernameTaken(username))
            return AuthResult::USERNAME_TAKEN;
    }

    if (!email.isEmpty()) {
        if (user.getEmail() != email && !Validator::validateEmail(email.toStdString()))
            return AuthResult::INVALID_EMAIL;

        if (user.getEmail() != email && storage.isEmailTaken(email))
            return AuthResult::EMAIL_TAKEN;
    }

    if (!phone.isEmpty()) {
        if (user.getPhoneNumber() != phone && !Validator::validatePhone(phone.toStdString()))
            return AuthResult::INVALID_PHONE;

        if (user.getPhoneNumber() != phone && user.getPhoneNumber() != phone && storage.isPhoneNumberTaken(phone))
            return AuthResult::PHONE_TAKEN;
    }

    if (!old_password.isEmpty() && !new_password.isEmpty()) {
        if (!PasswordHasher::verify(old_password, user.getHashedPassword()))
            return AuthResult::WRONG_PASSWORD;

        if (old_password != new_password) {
            if (!Validator::validatePassword(new_password.toStdString()))
                return AuthResult::WEAK_PASSWORD;

            user.setHashedPassword(PasswordHasher::hasher(new_password));
        }
    }
    else
        return AuthResult::EMPTY_FIELD;

    user.setName(name);
    user.setUsername(username);
    user.setEmail(email);
    user.setPhoneNumber(phone);

    if (!storage.updateUser(user))
        return AuthResult::UNKNOWN_ERROR;

    SessionManager::getInstance().updateCurrentUser(user);

    return AuthResult::SUCCESS;
}