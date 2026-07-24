#include "authenticator.h"
#include "Logic/Security/validator.h"
#include "Logic/Security/password_hasher.h"
#include "Management/App/session_manager.h"

Authenticator::Authenticator(UserInterface& storage): storage(storage), session_storage(){
}

AuthResult Authenticator::login(const QString &username, const QString &password, User &logged_in_user)
{
    User user;

    if (!storage.getUserByUsername(username, user))
        return AuthResult::USER_NOT_FOUND;

    if (!PasswordHasher::verify(password, user.getHashedPassword()))
        return AuthResult::WRONG_PASSWORD;

    if (session_storage.isUserOnline(user.getId()))
        return AuthResult::ALREADY_LOGGED_IN;

    session_storage.addSession(user.getId());

    logged_in_user = user;
    return AuthResult::SUCCESS;
}

void Authenticator::logout(int user_id)
{
    session_storage.removeSession(user_id);
}

AuthResult Authenticator::signup(const QString &name, const QString &username, const QString &email, const QString &phone, const QString &password)
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

    User new_user(name, username, phone, email, hashedPassword);

    if (!storage.addUser(new_user))
        return AuthResult::UNKNOWN_ERROR;

    return AuthResult::SUCCESS;
}

AuthResult Authenticator::resetPassword(const QString &username, const QString &phone, const QString &new_password)
{
    User user;

    if (!storage.getUserByUsername(username, user))
        return AuthResult::USER_NOT_FOUND;

    if (user.getPhoneNumber() != phone)
        return AuthResult::USER_PHONE_MISMATCH;

    if (!Validator::validatePassword(new_password.toStdString()))
        return AuthResult::WEAK_PASSWORD;

    QString hashedPassword = PasswordHasher::hasher(new_password);

    user.setHashedPassword(hashedPassword);

    if (!storage.updateUser(user))
        return AuthResult::UNKNOWN_ERROR;

    return AuthResult::SUCCESS;
}

AuthResult Authenticator::verifyUserPhone(const QString &username, const QString &phone)
{
    User user;

    if (!storage.getUserByUsername(username, user))
        return AuthResult::USER_NOT_FOUND;

    if (user.getPhoneNumber() != phone)
        return AuthResult::USER_PHONE_MISMATCH;

    return AuthResult::SUCCESS;
}

AuthResult Authenticator::updateUser(const int id, const QString &name, const QString &username, const QString &email, const QString &phone, const QString &old_password, const QString &new_password)
{
    User user;

    if (!username.isEmpty())
    {
        if (!storage.getUserById(id, user))
            return AuthResult::USER_NOT_FOUND;

        if (user.getUsername() != username && storage.isUsernameTaken(username))
            return AuthResult::USERNAME_TAKEN;
    }

    if (!email.isEmpty())
    {
        if (user.getEmail() != email && !Validator::validateEmail(email.toStdString()))
            return AuthResult::INVALID_EMAIL;

        if (user.getEmail() != email && storage.isEmailTaken(email))
            return AuthResult::EMAIL_TAKEN;
    }

    if (!phone.isEmpty())
    {
        if (user.getPhoneNumber() != phone && !Validator::validatePhone(phone.toStdString()))
            return AuthResult::INVALID_PHONE;

        if (user.getPhoneNumber() != phone && storage.isPhoneNumberTaken(phone))
            return AuthResult::PHONE_TAKEN;
    }

    if (!old_password.isEmpty() && !new_password.isEmpty())
    {
        if (!PasswordHasher::verify(old_password, user.getHashedPassword()))
            return AuthResult::WRONG_PASSWORD;

        if (old_password != new_password)
        {
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

AuthResult Authenticator::verifyIP(const QString& ip)
{
    if (!Validator::validateIP(ip.toStdString()))
        return AuthResult::INVALID_IP;

    return AuthResult::SUCCESS;
}