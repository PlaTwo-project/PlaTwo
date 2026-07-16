#ifndef FANORONA_MOVE_H
#define FANORONA_MOVE_H

#include "Logic/Game/BaseLogicClasses/Move.h"
#include "Logic/Game/Fanorona/Fanoronaboard/fanorona_board.h"
#include <QString>

class FanoronaMove : public Move
{
public:
    FanoronaMove(int from, int to, int player_id, FanoronaCaptureType capture_type, bool is_end_turn = false);
    ~FanoronaMove() override;

    QByteArray serializeMove() const override;

    int getFrom() const;
    int getTo() const;
    int getPlayerId() const;
    FanoronaCaptureType getCaptureType() const;
    bool isEndTurn() const;

private:
    int from_position;
    int to_position;
    int player_id;
    FanoronaCaptureType capture_type;
    bool end_turn;
};

#endif // FANORONA_MOVE_H