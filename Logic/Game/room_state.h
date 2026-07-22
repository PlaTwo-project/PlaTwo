#ifndef ROOM_STATE_H
#define ROOM_STATE_H

#include "Logic/User/user.h"
#include "Logic/Game/game_name.h"

class RoomState
{
public:
    RoomState(int port, const User& host_user, const User& guest_user, GameName game_name,const QString& host_ip, int board_size, int time_limit);

    int getPort() const;
    User getHostUser() const;
    User getGuestUser() const;
    GameName getGameName() const;
    QString getHostIp() const;
    int getBoardSize() const;
    int getTimeLimit() const;
    int getDuration() const;
    int getHostColorIndex() const;
    int getGuestColorIndex() const;

    void setPort(int port);
    void setHostUser(const User& host_user);
    void setGuestUser(const User& guest_user);
    void setGameName(GameName game_name);
    void setHostIp(const QString& host_ip);
    void setBoardSize(int board_size);
    void setTimeLimit(int time_limit);
    void setDuration(int game_duration);
    void setHostColorIndex(int color_index);
    void setGuestColorIndex(int color_index);

private:
    int port;
    User host_user;
    User guest_user;
    GameName game_name;
    QString host_ip;
    int board_size;
    int time_limit;
    int game_duration;
    int host_color_index = -1;
    int guest_color_index = -1;
};

#endif // ROOM_STATE_H