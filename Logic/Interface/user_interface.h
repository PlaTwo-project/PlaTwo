#ifndef IUSERREPOSITORY_H
#define IUSERREPOSITORY_H

#include "Logic/User/user.h"

class UserInterface
{
public:
    virtual ~UserInterface() = default;

    virtual bool addUser(User& new_user) = 0;
    virtual bool updateUser(const User& user_to_update) = 0;

    virtual bool isUsernameTaken(const QString& username) const = 0;
    virtual bool isEmailTaken(const QString& email) const = 0;
    virtual bool isPhoneNumberTaken(const QString& phone) const = 0;

    virtual bool getUserByUsername(const QString& username, User& user_to_find) const = 0;
    virtual bool getUserByPhoneNumber(const QString& phone, User& user_to_find) const = 0;
    virtual bool getUserById(const int& id, User& user_to_find) const = 0;
};

#endif // IUSERREPOSITORY_H