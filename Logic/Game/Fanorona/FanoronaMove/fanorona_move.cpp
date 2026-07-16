#include "fanorona_move.h"

FanoronaMove::FanoronaMove(int from, int to, int player_id, FanoronaCaptureType capture_type, bool is_end_turn)
    : from_position(from), to_position(to), player_id(player_id), capture_type(capture_type), end_turn(is_end_turn)
{
}

FanoronaMove::~FanoronaMove()
{
}

QByteArray FanoronaMove::serializeMove() const
{
    return QString("%1,%2,%3,%4,%5")
    .arg(from_position)
        .arg(to_position)
        .arg(player_id)
        .arg(static_cast<int>(capture_type))
        .arg(end_turn ? 1 : 0)
        .toUtf8();
}

int FanoronaMove::getFrom() const {
    return from_position;
}
int FanoronaMove::getTo() const {
    return to_position;
}
int FanoronaMove::getPlayerId() const {
    return player_id;
}
FanoronaCaptureType FanoronaMove::getCaptureType() const {
    return capture_type;
}
bool FanoronaMove::isEndTurn() const {
    return end_turn;
}