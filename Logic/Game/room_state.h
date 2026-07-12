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

    void setPort(int port);
    void setHostUser(const User& host_user);
    void setGuestUser(const User& guest_user);
    void setGameName(GameName game_name);
    void setHostIp(const QString& host_ip);
    void setBoardSize(int board_size);
    void setTimeLimit(int time_limit);

private:
    int port;
    User host_user;
    User guest_user;
    GameName game_name;
    QString host_ip;
    int board_size;
    int time_limit;
};

#endif // ROOM_STATE_H