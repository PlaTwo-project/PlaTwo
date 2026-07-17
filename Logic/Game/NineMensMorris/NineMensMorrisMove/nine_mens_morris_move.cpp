#include "nine_mens_morris_move.h"

NineMensMorrisMove::NineMensMorrisMove(MoveType move_type, int player_id, int from_position, int to_position)
    : move_type(move_type), player_id(player_id), from_position(from_position), to_position(to_position) {}

QByteArray NineMensMorrisMove::serializeMove() const {
    return QString("%1,%2,%3,%4").arg(static_cast<int>(move_type)).arg(player_id).arg(from_position).arg(to_position).toUtf8();
}

MoveType NineMensMorrisMove::getMoveType() const {
    return move_type;
}

int NineMensMorrisMove::getPlayerId() const {
    return player_id;
}

int NineMensMorrisMove::getFromPosition() const {
    return from_position;
}

int NineMensMorrisMove::getToPosition() const {
    return to_position;
}