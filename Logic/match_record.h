#ifndef MATCH_RECORD_H
#define MATCH_RECORD_H

#include <QString>
#include <QJsonObject>

class MatchRecord
{
private:
    int recordId;
    QString gameType;
    QString hostUsername;
    QString guestUsername;
    QString winnerUsername;
    int hostScore;
    int guestScore;
    QString date;

public:
    MatchRecord();
    MatchRecord(const QString& type, const QString& host, const QString& guest, const QString& winner, int hostS, int guestS, const QString& gameDate);


    int getRecordId() const;
    QString getGameType() const;
    QString getHostUsername() const;
    QString getGuestUsername() const;
    QString getWinnerUsername() const;
    int getHostScore() const;
    int getGuestScore() const;
    QString getDate() const;


    void setRecordId(int id);
    void setGameType(const QString& type);
    void setHostUsername(const QString& host);
    void setGuestUsername(const QString& guest);
    void setWinnerUsername(const QString& winner);
    void setHostScore(int score);
    void setGuestScore(int score);
    void setDate(const QString& gameDate);
};

#endif // MATCH_RECORD_H