#ifndef MATCH_RECORD_H
#define MATCH_RECORD_H

#include <QString>
#include <QDateTime>
#include "Logic/Game/game_name.h"

class MatchRecord
{
private:
    int record_id;
    GameName game_type;
    int host_id;
    int guest_id;
    int winner_id;
    int host_score;
    int guest_score;
    QDateTime date;

public:
    MatchRecord();
    MatchRecord(GameName game_name, int host, int guest, int winner, int hostS, int guestS, const QDateTime &gameDate);

    int getRecordId() const;
    GameName getGameType() const;
    int getHostId() const;
    int getGuestId() const;
    int getWinnerId() const;
    int getHostScore() const;
    int getGuestScore() const;
    QDateTime getDate() const;

    void setRecordId(int id);
    void setGameType(GameName game_name);
    void setHostId(int host);
    void setGuestId(int guest);
    void setWinnerId(int winner);
    void setHostScore(int score);
    void setGuestScore(int score);
    void setDate(const QDateTime &gameDate);
};

#endif // MATCH_RECORD_H