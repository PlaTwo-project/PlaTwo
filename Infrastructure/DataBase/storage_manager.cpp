#include "storage_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

StorageManager::StorageManager() {
    file_path = "users.json";
    loadUsers();
}

void StorageManager::loadUsers() {
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray jsonArray = doc.array();

    users_list.clear();

    for (const QJsonValue& value : jsonArray) {
        QJsonObject obj = value.toObject();

        User user;
        user.setName(obj["username"].toString());
        user.setEmail(obj["email"].toString());
        user.setPhoneNumber(obj["phone"].toString());
        user.setHashedPassword(obj["password"].toString());

        users_list.append(user);
    }
    file.close();
}

void StorageManager::saveUsers(){
    //ino benvis
}

bool StorageManager::addUser(const User& newUser) {
    if (isUsernameTaken(newUser.getName()) || isEmailTaken(newUser.getEmail()) || isPhoneNumberTaken(newUser.getPhoneNumber()) )
        return false;

    users_list.append(newUser);
    saveUsers();
    return true;
}

bool StorageManager::updateUser(const User& user_to_update) {
    //ino benvis
}


bool StorageManager::isUsernameTaken(const QString& username) const {
    for (const User& user : users_list) {
        if (user.getName() == username) {
            return true;
        }
    }
    return false;
}

bool StorageManager::isEmailTaken(const QString& email) const {
    //ino benvis
}
bool StorageManager::isPhoneNumberTaken(const QString& phone) const {
    //ino benvis
}
bool StorageManager::getUserByUsername(const QString& username, User& user_to_find) const {
    //ino benvis
}
bool StorageManager::getUserByPhoneNumber(const QString& phone, User& user_to_find) const {
    for (const User& tmp_user : users_list) {
        if (tmp_user.getPhoneNumber() == phone) {
            user_to_find = tmp_user;
            return true;
        }
    }
    return false;
}