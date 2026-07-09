#ifndef ROOMSTATE_H
#define ROOMSTATE_H

#include <QString>
#include "Logic/User/user.h"
#include "Logic/Game/game_name.h"

class RoomState
{
public:
    RoomState(int port, const User& host_user, const User& guest_user, GameName game_name,const QString& host_ip, int board_size, int time_limit);

    int getPort() const;
    void setPort(int port);

    User getHostUser() const;
    void setHostUser(const User& host_user);

    User getGuestUser() const;
    void setGuestUser(const User& guest_user);

    GameName getGameName() const;
    void setGameName(GameName game_name);

    QString getHostIp() const;
    void setHostIp(const QString& host_ip);

    int getBoardSize() const;
    void setBoardSize(int board_size);

    int getTimeLimit() const;
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

#endif // ROOMSTATE_H