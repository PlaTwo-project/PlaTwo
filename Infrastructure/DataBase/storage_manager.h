#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <QList>
#include "Logic/User/user.h"
#include "Logic/Interface/user_interface.h"

class StorageManager : public UserInterface
{
public:
    StorageManager(const QString& file_path = "users.json");

    bool addUser(User &new_user) override;
    bool updateUser(const User &user_to_update) override;

    bool isUsernameTaken(const QString &username) const override;
    bool isEmailTaken(const QString &email) const override;
    bool isPhoneNumberTaken(const QString &phone) const override;

    bool getUserByUsername(const QString &username, User &user_to_find) const override;
    bool getUserByPhoneNumber(const QString &phone, User &user_to_find) const override;
    bool getUserById(const int& id, User& user_to_find) const override;

private:
    QString file_path;
    QList<User> users_list;

    void loadUsers();
    void saveUsers();
    int generateNextUserId() const;
};

#endif // STORAGE_MANAGER_H