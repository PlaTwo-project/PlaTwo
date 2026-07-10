#ifndef MOVE_H
#define MOVE_H

#include <QString>

class Move {
public:
    virtual ~Move() = default;
    virtual QString serializeMove() const = 0;
};

#endif // MOVE_H