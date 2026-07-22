#include "room_state.h"

RoomState::RoomState(int port, const User& host_user, const User& guest_user, GameName game_name,const QString& host_ip, int board_size, int time_limit)
    : port(port), host_user(host_user), guest_user(guest_user), game_name(game_name), host_ip(host_ip), board_size(board_size), time_limit(time_limit)
{
}

int RoomState::getPort() const
{
    return port;
}

void RoomState::setPort(int port)
{
    this->port = port;
}

User RoomState::getHostUser() const
{
    return host_user;
}

void RoomState::setHostUser(const User& host_user)
{
    this->host_user = host_user;
}

User RoomState::getGuestUser() const
{
    return guest_user;
}

void RoomState::setGuestUser(const User& guest_user)
{
    this->guest_user = guest_user;
}

GameName RoomState::getGameName() const
{
    return game_name;
}

void RoomState::setGameName(GameName game_name)
{
    this->game_name = game_name;
}

QString RoomState::getHostIp() const
{
    return host_ip;
}

void RoomState::setHostIp(const QString& host_ip)
{
    this->host_ip = host_ip;
}

int RoomState::getBoardSize() const
{
    return board_size;
}

void RoomState::setBoardSize(int board_size)
{
    this->board_size = board_size;
}

int RoomState::getTimeLimit() const
{
    return time_limit;
}

void RoomState::setTimeLimit(int time_limit)
{
    this->time_limit = time_limit;
}

void RoomState::setDuration (const int game_duration)
{
    this->game_duration = game_duration;
}

int RoomState::getDuration() const
{
    return game_duration;
}

int RoomState::getHostColorIndex() const {
    return host_color_index;
}

void RoomState::setHostColorIndex(int color_index) {
    this->host_color_index = color_index;
}

int RoomState::getGuestColorIndex() const {
    return guest_color_index;
}

void RoomState::setGuestColorIndex(int color_index) {
    this->guest_color_index = color_index;
}