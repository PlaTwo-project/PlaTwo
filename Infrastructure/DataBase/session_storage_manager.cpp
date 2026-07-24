#include "session_storage_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

SessionStorageManager::SessionStorageManager(): file_path("online_sessions.json")
{
    loadSessions();
}

void SessionStorageManager::loadSessions()
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray jsonArray = doc.array();

    online_users_list.clear();

    for (const QJsonValue &value : jsonArray)
    {
        QJsonObject obj = value.toObject();
        int user_id = obj["user_id"].toInt();

        if (!online_users_list.contains(user_id))
            online_users_list.append(user_id);
    }
    file.close();
}

void SessionStorageManager::saveSessions() const
{
    QJsonArray jsonArray;

    for (int user_id : online_users_list)
    {
        QJsonObject obj;
        obj["user_id"] = user_id;
        jsonArray.append(obj);
    }

    QJsonDocument doc(jsonArray);

    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    file.write(doc.toJson());
    file.close();
}

bool SessionStorageManager::addSession(int user_id)
{
    loadSessions();
    if (isUserOnline(user_id))
        return false;

    online_users_list.append(user_id);
    saveSessions();
    return true;
}

bool SessionStorageManager::removeSession(int user_id)
{
    loadSessions();
    bool removed = online_users_list.removeOne(user_id);
    if (removed)
        saveSessions();

    return removed;
}

bool SessionStorageManager::isUserOnline(int user_id)
{
    loadSessions();
    return online_users_list.contains(user_id);
}

void SessionStorageManager::clearAllSessions()
{
    online_users_list.clear();
    saveSessions();
}
