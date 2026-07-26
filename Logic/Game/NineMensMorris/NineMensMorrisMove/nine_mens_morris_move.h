#ifndef NINE_MENS_MORRIS_MOVE_H
#define NINE_MENS_MORRIS_MOVE_H

#include "Logic/Game/BaseLogicClasses/Move.h"
#include "Logic/Constants/player_slot.h"

enum class MoveType {
    PLACE,
    MOVE,
    REMOVE
};

class NineMensMorrisMove : public Move
{
public:
    NineMensMorrisMove(MoveType move_type, PlayerSlot player_id, int from_position, int to_position);
    ~NineMensMorrisMove() override = default;

    QByteArray serializeMove() const override;

    MoveType getMoveType() const;
    PlayerSlot getPlayerId() const;
    int getFromPosition() const;
    int getToPosition() const;

private:
    MoveType move_type;
    PlayerSlot player_id;
    int from_position;
    int to_position;
};

#endif // NINE_MENS_MORRIS_MOVE_H