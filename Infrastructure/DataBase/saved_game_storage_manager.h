#ifndef SAVED_GAME_STORAGE_MANAGER_H
#define SAVED_GAME_STORAGE_MANAGER_H
#include <QList>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "Logic/Game/game_name.h"
#include "Logic/Game/saved_game.h"

class SavedGameStorageManager
{
public:
    SavedGameStorageManager();

    void loadSavedGames();
    void saveSavedGames() const;

    void saveOrUpdateGame(GameName game_name, int host_id, int guest_id, int board_size, int time_limit, int elapsed_time, const QString &state_data);

    bool findSavedGame(int host_id, int guest_id, GameName game_name, SavedGame &out_record) const;
    void removeSavedGame(int host_id, int guest_id, GameName game_name);

private:
    QString file_path;
    QList<SavedGame> saved_games_list;
};

#endif // SAVED_GAME_STORAGE_MANAGER_H
