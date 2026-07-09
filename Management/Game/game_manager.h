#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include "Logic/Game/room_state.h"
#include "Infrastructure/Network/guest.h"
#include "Infrastructure/Network/host.h"

enum class Role
{
    Default,
    Host,
    Guest
};

class GameManager : public QObject
{
    Q_OBJECT

public:
    explicit GameManager(QObject *parent = nullptr);
    ~GameManager();

    bool createRoom(const User& host_user, int port, GameName game_name, int board_size, int time_limit);
    bool joinRoom(const User& guest_user, const QString& host_ip, int port);
    bool cancelRoom();

    Role getRole() const;

private:
    Role role;
    RoomState* room_state;
    Host* host;
    Guest* guest;
};

#endif // GAMEMANAGER_H
