#include "game_manager.h"

GameManager::GameManager(QObject *parent): QObject(parent), role(Role::Default), room_state(nullptr), host(nullptr), guest(nullptr)
{
}

GameManager::~GameManager()
{
    delete room_state;
}

bool GameManager::createRoom(const User& host_user, int port, GameName game_name, int board_size, int time_limit)
{
    if (role != Role::Default)
        return false;

    role = Role::Host;
    host = new Host(this);
    room_state = new RoomState(port, host_user, User(), game_name, QString(), board_size, time_limit);

    host->startHosting(port);

    return true;
}

bool GameManager::joinRoom(const User& guest_user, const QString& host_ip, int port)
{
    if (role != Role::Default)
        return false;

    role = Role::Guest;
    guest = new Guest(this);
    room_state = new RoomState(port, User(), guest_user, GameName::Default, host_ip, 0, 0);

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

    role = Role::Default;

    return true;
}

Role GameManager::getRole() const
{
    return role;
}