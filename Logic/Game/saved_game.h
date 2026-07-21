#ifndef SAVED_GAME_H
#define SAVED_GAME_H
#include "Logic/Game/game_name.h"
#include <QString>

class SavedGame
{
public:
    SavedGame();
    SavedGame(int game_id, GameName game_name, int host_id, int guest_id, int board_size, int time_limit, int elapsed_time, const QString &state_data);

    int getGameId() const;
    void setGameId(int game_id);

    GameName getGameName() const;
    void setGameName(GameName game_name);

    int getHostId() const;
    void setHostId(int host_id);

    int getGuestId() const;
    void setGuestId(int guest_id);

    int getBoardSize() const;
    void setBoardSize(int board_size);

    int getTimeLimit() const;
    void setTimeLimit(int time_limit);

    int getElapsedTime() const;
    void setElapsedTime(int elapsed_time);

    QString getStateData() const;
    void setStateData(const QString &state_data);

private:
    int game_id;
    GameName game_name;
    int host_id;
    int guest_id;
    int board_size;
    int time_limit;
    int elapsed_time;
    QString state_data;
};
#endif // SAVED_GAME_H