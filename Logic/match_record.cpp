#include "match_record.h"

MatchRecord::MatchRecord()
    : recordId(0), game_type(""), hostId(0), guestId(0), winnerId(0), hostScore(0), guestScore(0), date(QDateTime())
{
}

MatchRecord::MatchRecord(const QString &type, int host, int guest, int winner, int hostS, int guestS, const QDateTime &gameDate)
    : recordId(0), game_type(type), hostId(host), guestId(guest), winnerId(winner), hostScore(hostS), guestScore(guestS), date(gameDate)
{
}

int MatchRecord::getRecordId() const
{
    return recordId;
}
QString MatchRecord::getGameType() const
{
    return game_type;
}
int MatchRecord::getHostId() const
{
    return hostId;
}
int MatchRecord::getGuestId() const
{
    return guestId;
}
int MatchRecord::getWinnerId() const
{
    return winnerId;
}
int MatchRecord::getHostScore() const
{
    return hostScore;
}
int MatchRecord::getGuestScore() const
{
    return guestScore;
}
QDateTime MatchRecord::getDate() const
{
    return date;
}

void MatchRecord::setRecordId(int id)
{
    recordId = id;
}
void MatchRecord::setGameType(const QString &type)
{
    game_type = type;
}
void MatchRecord::setHostId(int host)
{
    hostId = host;
}
void MatchRecord::setGuestId(int guest)
{
    guestId = guest;
}
void MatchRecord::setWinnerId(int winner)
{
    winnerId = winner;
}
void MatchRecord::setHostScore(int score)
{
    hostScore = score;
}
void MatchRecord::setGuestScore(int score)
{
    guestScore = score;
}
void MatchRecord::setDate(const QDateTime &gameDate)
{
    date = gameDate;
}