#ifndef SAVED_GAME_H
#define SAVED_GAME_H
#include "Logic/Game/game_name.h"
#include <QString>

class SavedGame
{
public:
    SavedGame();
    SavedGame(GameName game_name, int host_id, int guest_id, int board_size, int time_limit, int elapsed_time, int host_elapsed_time, int guest_elapsed_time, const QString &state_data);

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

    int getHostElapsed() const;
    void setHostElapsed(int elapsed_time);

    int getGuestElapsed() const;
    void setGuestElapsed(int elapsed_time);

    QString getStateData() const;
    void setStateData(const QString &state_data);

private:
    GameName game_name;
    int host_id;
    int guest_id;
    int board_size;
    int time_limit;
    int elapsed_time;
    int host_elapsed_time;
    int guest_elapsed_time;
    QString state_data;
};
#endif // SAVED_GAME_H