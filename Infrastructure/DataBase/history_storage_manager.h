#ifndef HISTORY_STORAGE_MANAGER_H
#define HISTORY_STORAGE_MANAGER_H

#include <QString>
#include <QList>
#include "Logic/match_record.h"

class HistoryStorageManager
{
public:
    HistoryStorageManager();

    bool addMatchRecord(MatchRecord &new_record);
    QList<MatchRecord> getHistoryForUser(int user_ID, GameName game_type) const;

private:
    QString file_path;
    QList<MatchRecord> history_list;

    void loadHistory();
    void saveHistory();
    int generateNextRecordId() const;
};

#endif // HISTORY_STORAGE_MANAGER_H