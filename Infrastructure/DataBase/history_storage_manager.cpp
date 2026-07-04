#include "history_storage_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

HistoryStorageManager::HistoryStorageManager()
{
    file_path = "history.json";
    loadHistory();
}

int HistoryStorageManager::generateNextRecordId() const
{
    int max_id = 0;
    for (const MatchRecord &record : history_list)
        if (record.getRecordId() > max_id)
            max_id = record.getRecordId();
    return max_id + 1;
}

void HistoryStorageManager::loadHistory()
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray json_array = doc.array();
    history_list.clear();

    for (const QJsonValue &value : json_array) {
        QJsonObject obj = value.toObject();

        MatchRecord record;
        record.setRecordId(obj["id"].toInt());
        record.setGameType(obj["game_type"].toString());
        record.setHostId(obj["host_id"].toInt());
        record.setGuestId(obj["guest_id"].toInt());
        record.setWinnerId(obj["winner_id"].toInt());
        record.setHostScore(obj["host_score"].toInt());
        record.setGuestScore(obj["guest_score"].toInt());

        if (obj.contains("date") && obj["date"].isString())
            record.setDate(QDateTime::fromString(obj["date"].toString(), Qt::ISODate));

        history_list.append(record);
    }
}

void HistoryStorageManager::saveHistory()
{
    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly))
        return;

    QJsonArray json_array;
    for (const MatchRecord &record : history_list) {
        QJsonObject obj;
        obj["id"] = record.getRecordId();
        obj["game_type"] = record.getGameType();
        obj["host_id"] = record.getHostId();
        obj["guest_id"] = record.getGuestId();
        obj["winner_id"] = record.getWinnerId();
        obj["host_score"] = record.getHostScore();
        obj["guest_score"] = record.getGuestScore();
        obj["date"] = record.getDate().toString(Qt::ISODate);

        json_array.append(obj);
    }

    QJsonDocument doc(json_array);
    file.write(doc.toJson());
    file.close();
}

bool HistoryStorageManager::addMatchRecord(MatchRecord &newRecord)
{
    int new_id = generateNextRecordId();
    newRecord.setRecordId(new_id);
    history_list.append(newRecord);

    saveHistory();

    return true;
}

QList<MatchRecord> HistoryStorageManager::getHistoryForUser(int userId, const QString &game_type) const
{
    QList<MatchRecord> filtered_list;
    for (const MatchRecord &record : history_list)
        if (record.getGameType() == game_type && (record.getHostId() == userId || record.getGuestId() == userId))
            filtered_list.append(record);

    return filtered_list;
}