#include "match_record.h"


MatchRecord::MatchRecord()
    : recordId(0), gameType(""), hostUsername(""), guestUsername(""),
    winnerUsername(""), hostScore(0), guestScore(0), date("")
{}

MatchRecord::MatchRecord(const QString& type, const QString& host, const QString& guest, const QString& winner, int hostS, int guestS, const QString& gameDate)
    : recordId(0), gameType(type), hostUsername(host), guestUsername(guest), winnerUsername(winner), hostScore(hostS), guestScore(guestS), date(gameDate)
{}


int MatchRecord::getRecordId() const {
    return recordId;
}
QString MatchRecord::getGameType() const {
    return gameType;
}
QString MatchRecord::getHostUsername() const {
    return hostUsername;
}
QString MatchRecord::getGuestUsername() const {
    return guestUsername;
}
QString MatchRecord::getWinnerUsername() const {
    return winnerUsername;
}
int MatchRecord::getHostScore() const {
    return hostScore;
}
int MatchRecord::getGuestScore() const {
    return guestScore;
}
QString MatchRecord::getDate() const {
    return date;
}

void MatchRecord::setRecordId(int id) {
    recordId = id;
}
void MatchRecord::setGameType(const QString& type) {
    gameType = type;
}
void MatchRecord::setHostUsername(const QString& host) {
    hostUsername = host;
}
void MatchRecord::setGuestUsername(const QString& guest) {
    guestUsername = guest;
}
void MatchRecord::setWinnerUsername(const QString& winner) {
    winnerUsername = winner;
}
void MatchRecord::setHostScore(int score) {
    hostScore = score;
}
void MatchRecord::setGuestScore(int score) {
    guestScore = score;
}
void MatchRecord::setDate(const QString& gameDate) {
    date = gameDate;
}