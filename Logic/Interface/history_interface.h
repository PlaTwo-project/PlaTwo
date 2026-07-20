#pragma once

#include <QList>
#include "Logic/Game/Record/match_record.h"

class HistoryInterface
{
public:
    virtual ~HistoryInterface() = default;

    virtual bool addMatchRecord(MatchRecord& new_record) = 0;
    virtual QList<MatchRecord> getHistoryForUser(const int user_ID, const GameName game_type) const = 0;
    virtual void reloadFromDisk() = 0;
};