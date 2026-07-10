#ifndef BOARD_H
#define BOARD_H

#include "Logic/Game/BaseLogicClasses/Move.h"

class Board {
public:
    virtual ~Board() = default;
    virtual void initialize() = 0;
    virtual void clear() = 0;
    virtual void applyMove(const Move& move) = 0;
};

#endif // BOARD_H