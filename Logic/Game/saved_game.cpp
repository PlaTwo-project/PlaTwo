#include "saved_game.h"

SavedGame::SavedGame(): game_name(static_cast<GameName>(0)), host_id(0), guest_id(0), board_size(0), time_limit(0), elapsed_time(0), state_data("")
{
}

SavedGame::SavedGame( GameName game_name, int host_id, int guest_id,int board_size, int time_limit, int elapsed_time, const QString &state_data)
    : game_name(game_name), host_id(host_id), guest_id(guest_id), board_size(board_size), time_limit(time_limit), elapsed_time(elapsed_time), state_data(state_data)
{
}

GameName SavedGame::getGameName() const
{
    return game_name;
}

void SavedGame::setGameName(GameName game_name)
{
    this->game_name = game_name;
}

int SavedGame::getHostId() const
{
    return host_id;
}

void SavedGame::setHostId(int host_id)
{
    this->host_id = host_id;
}

int SavedGame::getGuestId() const
{
    return guest_id;
}

void SavedGame::setGuestId(int guest_id)
{
    this->guest_id = guest_id;
}

int SavedGame::getBoardSize() const
{
    return board_size;
}

void SavedGame::setBoardSize(int board_size)
{
    this->board_size = board_size;
}

int SavedGame::getTimeLimit() const
{
    return time_limit;
}

void SavedGame::setTimeLimit(int time_limit)
{
    this->time_limit = time_limit;
}

int SavedGame::getElapsedTime() const
{
    return elapsed_time;
}

void SavedGame::setElapsedTime(int elapsed_time)
{
    this->elapsed_time = elapsed_time;
}

QString SavedGame::getStateData() const
{
    return state_data;
}

void SavedGame::setStateData(const QString &state_data)
{
    this->state_data = state_data;
}