#include "storage_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

StorageManager::StorageManager(const QString& file_path)
{
    this->file_path = "users.json";
    loadUsers();
}

int StorageManager::generateNextUserId() const
{
    int max_id = 0;

    for (const User &user : users_list)
        if (user.getId() > max_id)
            max_id = user.getId();

    return max_id + 1;
}

void StorageManager::loadUsers()
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray jsonArray = doc.array();

    users_list.clear();

    for (const QJsonValue &value : jsonArray)
    {
        QJsonObject obj = value.toObject();

        User user;
        user.setId(obj["id"].toInt());
        user.setName(obj["name"].toString());
        user.setUsername(obj["username"].toString());
        user.setEmail(obj["email"].toString());
        user.setPhoneNumber(obj["phone"].toString());
        user.setHashedPassword(obj["password"].toString());

        users_list.append(user);
    }
    file.close();
}

void StorageManager::saveUsers()
{
    QJsonArray jsonArray;

    for (const User &user : users_list)
    {
        QJsonObject obj;
        obj["id"] = user.getId();
        obj["name"] = user.getName();
        obj["username"] = user.getUsername();
        obj["email"] = user.getEmail();
        obj["phone"] = user.getPhoneNumber();
        obj["password"] = user.getHashedPassword();

        jsonArray.append(obj);
    }

    QJsonDocument doc(jsonArray);

    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return;
    }

    file.write(doc.toJson());
    file.close();
}

bool StorageManager::addUser(User &new_user)
{
    if (isUsernameTaken(new_user.getUsername()) || isEmailTaken(new_user.getEmail()) || isPhoneNumberTaken(new_user.getPhoneNumber()))
        return false;

    new_user.setId(generateNextUserId());
    users_list.append(new_user);

    saveUsers();
    return true;
}

bool StorageManager::updateUser(const User &user_to_update)
{
    for (User &user : users_list)
    {
        if (user.getId() == user_to_update.getId())
        {
            if (user.getUsername() != user_to_update.getUsername())
                if (isUsernameTaken(user_to_update.getUsername()))
                    return false;

            if (user.getEmail() != user_to_update.getEmail())
                if (isEmailTaken(user_to_update.getEmail()))
                    return false;

            if (user.getPhoneNumber() != user_to_update.getPhoneNumber())
                if (isPhoneNumberTaken(user_to_update.getPhoneNumber()))
                    return false;

            user = user_to_update;
            saveUsers();
            return true;
        }
    }

    return false;
}

bool StorageManager::isUsernameTaken(const QString &username) const
{
    for (const User &user : users_list)
    {
        if (user.getUsername() == username)
        {
            return true;
        }
    }
    return false;
}

bool StorageManager::isEmailTaken(const QString &email) const
{
    for (const User &user : users_list)
    {
        if (user.getEmail() == email)
        {
            return true;
        }
    }
    return false;
}

bool StorageManager::isPhoneNumberTaken(const QString &phone) const
{
    for (const User &user : users_list)
    {
        if (user.getPhoneNumber() == phone)
        {
            return true;
        }
    }
    return false;
}

bool StorageManager::getUserByUsername(const QString &username, User &user_to_find) const
{
    for (const User &tmp_user : users_list)
    {
        if (tmp_user.getUsername() == username)
        {
            user_to_find = tmp_user;
            return true;
        }
    }
    return false;
}

bool StorageManager::getUserByPhoneNumber(const QString &phone, User &user_to_find) const
{
    for (const User &tmp_user : users_list)
    {
        if (tmp_user.getPhoneNumber() == phone)
        {
            user_to_find = tmp_user;
            return true;
        }
    }
    return false;
}

bool StorageManager::getUserById(const int &id, User &user_to_find) const
{
    for (const User &tmp_user : users_list)
    {
        if (tmp_user.getId() == id)
        {
            user_to_find = tmp_user;
            return true;
        }
    }
    return false;
}