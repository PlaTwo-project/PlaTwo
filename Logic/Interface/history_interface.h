#ifndef IHISTORYREPOSITORY_H
#define IHISTORYREPOSITORY_H

#include <QList>
#include "Logic/Game/Record/match_record.h"

class HistoryInterface
{
public:
    virtual ~HistoryInterface() = default;

    virtual bool addMatchRecord(MatchRecord& new_record) = 0;
    virtual QList<MatchRecord> getHistoryForUser(int user_ID, GameName& game_type) const = 0;
};

#endif // IHISTORYREPOSITORY_H