#include "game_manager.h"

GameManager::GameManager(QObject *parent): QObject(parent), room_state(nullptr), host(nullptr), guest(nullptr)
{}

GameManager::~GameManager()
{
    delete room_state;
}

QString GameManager::createRoom(const User& host_user, const int& port, const GameName& game_name, const int& board_size, const int& time_limit)
{
    role = Role::Host;
    host = new Host(this);
    QString ip = "127.0.0.1";
    room_state = new RoomState(port, host_user, User(), game_name, ip, board_size, time_limit);
    host->startHosting(port);

    return ip;
}

bool GameManager::joinRoom(const User& guest_user, const QString& host_ip, const int& port, const GameName& game_name)
{
    role = Role::Guest;
    guest = new Guest(this);
    room_state = new RoomState(port, User(), guest_user, game_name, host_ip, 0, 0);
    guest->connectHost(host_ip, port);

    return true;
}

bool GameManager::cancelRoom()
{
    if (role != Role::Host)
        return false;

    host->deleteLater();
    host = nullptr;
    delete room_state;
    room_state = nullptr;

    return true;
}

Role GameManager::getRole() const
{
    return role;
}