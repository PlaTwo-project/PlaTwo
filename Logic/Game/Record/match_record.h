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
    QString host_username;
    int guest_id;
    QString guest_username;
    int winner_id;
    int host_score;
    int guest_score;
    QDateTime date;
    int duration;

public:
    MatchRecord();
    MatchRecord(GameName game_name, int host_id, const QString& host_username, int guest, const QString& guest_username, int winner, int hostS, int guestS, QDateTime &gameDate, int gameDuration);

    int getRecordId() const;
    GameName getGameType() const;
    int getHostId() const;
    QString getHostUsername() const;
    int getGuestId() const;
    QString getGuestUsername() const;
    int getWinnerId() const;
    int getHostScore() const;
    int getGuestScore() const;
    QDateTime getDate() const;
    int getDuration() const;

    void setRecordId(int id);
    void setGameType(GameName game_name);
    void setHostId(int host);
    void setGuestId(int guest);
    void setHostUsername(const QString& username);
    void setGuestUsername(const QString& username);
    void setWinnerId(int winner);
    void setHostScore(int score);
    void setGuestScore(int score);
    void setDate(const QDateTime &gameDate);
    void setDuration(int time);
};

#endif // MATCH_RECORD_H