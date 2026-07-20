#include "match_record.h"

MatchRecord::MatchRecord()
    : record_id(0), game_type(), host_id(0), guest_id(0), winner_id(0), host_score(0), guest_score(0), date(QDateTime()), duration(0)
{
}

MatchRecord::MatchRecord(GameName game_name, int host, int guest, int winner, int hostS, int guestS,QDateTime &gameDate, int gameDuration)
    : record_id(0), game_type(game_name), host_id(host), guest_id(guest), winner_id(winner), host_score(hostS), guest_score(guestS), date(gameDate), duration(gameDuration)
{
}

int MatchRecord::getRecordId() const
{
    return record_id;
}

GameName MatchRecord::getGameType() const
{
    return game_type;
}

int MatchRecord::getHostId() const
{
    return host_id;
}

int MatchRecord::getGuestId() const
{
    return guest_id;
}

int MatchRecord::getWinnerId() const
{
    return winner_id;
}

int MatchRecord::getHostScore() const
{
    return host_score;
}

int MatchRecord::getGuestScore() const
{
    return guest_score;
}

QDateTime MatchRecord::getDate() const
{
    return date;
}

int MatchRecord::getDuration() const {
    return duration;
}

void MatchRecord::setRecordId(int id)
{
    record_id = id;
}

void MatchRecord::setGameType(GameName game_name)
{
    game_type = game_name;
}

void MatchRecord::setHostId(int host)
{
    host_id = host;
}

void MatchRecord::setGuestId(int guest)
{
    guest_id = guest;
}

void MatchRecord::setWinnerId(int winner)
{
    winner_id = winner;
}

void MatchRecord::setHostScore(int score)
{
    host_score = score;
}

void MatchRecord::setGuestScore(int score)
{
    guest_score = score;
}

void MatchRecord::setDate(const QDateTime &gameDate)
{
    date = gameDate;
}

void MatchRecord::setDuration(int time) {
    duration = time;
}