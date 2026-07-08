#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <QString>
#include <QList>
#include "Logic/user.h"

class StorageManager
{
public:
    StorageManager();

    bool addUser(User &new_user);
    bool updateUser(const User &user_to_update);

    bool isUsernameTaken(const QString &username) const;
    bool isEmailTaken(const QString &email) const;
    bool isPhoneNumberTaken(const QString &phone) const;

    bool getUserByUsername(const QString &username, User &user_to_find) const;
    bool getUserByPhoneNumber(const QString &phone, User &user_to_find) const;
    bool getUserById(const int &id, User &user_to_find) const;

private:
    QString file_path;
    QList<User> users_list;

    void loadUsers();
    void saveUsers();
    int generateNextUserId() const;
};

#endif // STORAGE_MANAGER_H