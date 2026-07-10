#ifndef BOXES_AND_DOTS_MOVE_H
#define BOXES_AND_DOTS_MOVE_H

#include "Logic/Game/BaseLogicClasses/Move.h"
#include <QString>

enum class lineDirection
{
    HORIZONTAL,
    VERTICAL
};

class DotsAndBoxesMove : public Move
{
public:
    DotsAndBoxesMove(int row, int column, lineDirection direction);
    ~DotsAndBoxesMove() override;

    QString serializeMove() const override;

    int getRow() const;
    int getColumn() const;
    lineDirection getDirection() const;

private:
    int row_index;
    int column_index;
    lineDirection line_direction;
};

#endif // BOXES_AND_DOTS_MOVE_H