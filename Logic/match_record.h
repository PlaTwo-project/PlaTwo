#ifndef MATCH_RECORD_H
#define MATCH_RECORD_H

#include <QString>
#include <QDateTime>

class MatchRecord
{
private:
    int recordId;
    QString game_type;
    int hostId;
    int guestId;
    int winnerId;
    int hostScore;
    int guestScore;
    QDateTime date;

public:
    MatchRecord();
    MatchRecord(const QString &type, int host, int guest, int winner, int hostS, int guestS, const QDateTime &gameDate);

    int getRecordId() const;
    QString getGameType() const;
    int getHostId() const;
    int getGuestId() const;
    int getWinnerId() const;
    int getHostScore() const;
    int getGuestScore() const;
    QDateTime getDate() const;

    void setRecordId(int id);
    void setGameType(const QString &type);
    void setHostId(int host);
    void setGuestId(int guest);
    void setWinnerId(int winner);
    void setHostScore(int score);
    void setGuestScore(int score);
    void setDate(const QDateTime &gameDate);
};

#endif // MATCH_RECORD_H