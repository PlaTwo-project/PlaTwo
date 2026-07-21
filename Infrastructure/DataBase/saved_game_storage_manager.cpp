#include "saved_game_storage_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

SavedGameStorageManager::SavedGameStorageManager()
{
    file_path = "saved_games.json";
    loadSavedGames();
}

void SavedGameStorageManager::loadSavedGames()
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray array = doc.array();
    saved_games_list.clear();

    for (const QJsonValue &value : array)
    {
        QJsonObject obj = value.toObject();
        SavedGame record;

        record.setGameId(obj["game_id"].toInt());
        record.setGameName(static_cast<GameName>(obj["game_name"].toInt()));
        record.setHostId(obj["host_id"].toInt());
        record.setGuestId(obj["guest_id"].toInt());
        record.setBoardSize(obj["board_size"].toInt());
        record.setTimeLimit(obj["time_limit"].toInt());
        record.setElapsedTime(obj["elapsed_time"].toInt());
        record.setStateData(obj["state_data"].toString());

        saved_games_list.append(record);
    }
}

void SavedGameStorageManager::saveSavedGames() const
{
    QJsonArray array;
    for (const SavedGame &record : saved_games_list)
    {
        QJsonObject obj;
        obj["game_id"] = record.getGameId();
        obj["game_name"] = static_cast<int>(record.getGameName());
        obj["host_id"] = record.getHostId();
        obj["guest_id"] = record.getGuestId();
        obj["board_size"] = record.getBoardSize();
        obj["time_limit"] = record.getTimeLimit();
        obj["elapsed_time"] = record.getElapsedTime();
        obj["state_data"] = record.getStateData();

        array.append(obj);
    }

    QJsonDocument doc(array);
    QFile file(file_path);
    if (file.open(QIODevice::WriteOnly)){
        file.write(doc.toJson());
        file.close();
    }
}

void SavedGameStorageManager::saveOrUpdateGame(int game_id, GameName game_name, int host_id, int guest_id, int board_size, int time_limit, int elapsed_time, const QString &state_data)
{
    SavedGame newRecord(game_id, game_name, host_id, guest_id, board_size, time_limit, elapsed_time, state_data);

    for (int i = 0; i < saved_games_list.size(); ++i)
        if (saved_games_list[i].getHostId() == host_id && saved_games_list[i].getGuestId() == guest_id && saved_games_list[i].getGameName() == game_name){
            saved_games_list[i] = newRecord;
            saveSavedGames();
            return;
        }

    saved_games_list.append(newRecord);
    saveSavedGames();
}

bool SavedGameStorageManager::findSavedGame(int host_id, int guest_id, GameName game_name, SavedGame &out_record) const
{
    for (const auto &record : saved_games_list)
        if (record.getHostId() == host_id &&record.getGuestId() == guest_id && record.getGameName() == game_name){
            out_record = record;
            return true;
        }

    return false;
}

void SavedGameStorageManager::removeSavedGame(int host_id, int guest_id, GameName game_name)
{
    for (int i = 0; i < saved_games_list.size(); ++i)
        if (saved_games_list[i].getHostId() == host_id && saved_games_list[i].getGuestId() == guest_id && saved_games_list[i].getGameName() == game_name){
            saved_games_list.removeAt(i);
            saveSavedGames();
            return;
        }
}