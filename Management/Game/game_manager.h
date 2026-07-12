#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <QObject>
#include "Logic/Game/room_state.h"
#include "Infrastructure/Network/guest.h"
#include "Infrastructure/Network/host.h"

enum class Role{ Host, Guest };

class GameManager : public QObject
{
    Q_OBJECT

public:
    explicit GameManager(QObject *parent = nullptr);
    ~GameManager();

    QString createRoom(const User& host_user, const int& port, const GameName& game_name, const int& board_size, const int& time_limit);
    bool joinRoom(const User& guest_user, const QString& host_ip, const int& port, const GameName& game_name);
    bool cancelRoom();

    Role getRole() const;

private:
    Role role;
    RoomState* room_state;
    Host* host;
    Guest* guest;
};

#endif // GAME_MANAGER_H
