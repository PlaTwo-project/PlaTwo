#ifndef HISTORY_STORAGE_MANAGER_H
#define HISTORY_STORAGE_MANAGER_H

#include <QList>
#include "Logic/Game/Record/match_record.h"
#include "Logic/Interface/history_interface.h"

class HistoryStorageManager : public HistoryInterface
{
public:
    HistoryStorageManager();

    bool addMatchRecord(MatchRecord &new_record) override;
    QList<MatchRecord> getHistoryForUser(const int user_ID, const GameName game_type) const override;
    void reloadFromDisk() override;

private:
    QString file_path;
    QList<MatchRecord> history_list;

    void loadHistory();
    void saveHistory();
    int generateNextRecordId() const;
};

#endif // HISTORY_STORAGE_MANAGER_H