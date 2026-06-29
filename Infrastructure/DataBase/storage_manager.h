#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <QString>
#include <QList>
#include "Logic/user.h"

class StorageManager {
public:
    StorageManager(const QString filePath = "users.json");

    bool addUser(const User& newUser);
    bool updateUser(const User& user_to_update);

    bool isUsernameTaken(const QString& username) const;
    bool isEmailTaken(const QString& email) const;
    bool isPhoneNumberTaken(const QString& phone) const;

    bool getUserByUsername(const QString& username, User& user_to_find) const;
    bool getUserByPhoneNumber(const QString& phone, User& user_to_find) const;

private:
    QString file_path;
    QList<User> users_list;

    void loadUsers();
    void saveUsers();
};

#endif // STORAGE_MANAGER_H