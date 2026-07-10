#include "dots_and_boxes_move.h"

DotsAndBoxesMove::DotsAndBoxesMove(int row, int column, lineDirection direction)
    : row_index(row), column_index(column), line_direction(direction)
{
}

DotsAndBoxesMove::~DotsAndBoxesMove()
{
}

QString DotsAndBoxesMove::serializeMove() const
{
    return QString("%1,%2,%3").arg(row_index).arg(column_index).arg(static_cast<int>(line_direction));
}

int DotsAndBoxesMove::getRow() const
{
    return row_index;
}

int DotsAndBoxesMove::getColumn() const
{
    return column_index;
}

lineDirection DotsAndBoxesMove::getDirection() const
{
    return line_direction;
}