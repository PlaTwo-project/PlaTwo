#include "dots_and_boxes_board.h"
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesMove/dots_and_boxes_move.h"

DotsAndBoxesBoard::DotsAndBoxesBoard(int size) : board_size(size)
{
    initialize();
}

void DotsAndBoxesBoard::initialize()
{
    horizontal_lines.assign(board_size + 1, QVector<bool>(board_size, false));
    vertical_lines.assign(board_size, QVector<bool>(board_size + 1, false));
    captured_boxes.assign(board_size, QVector<int>(board_size, 0));
}

void DotsAndBoxesBoard::clear()
{
    initialize();
}

void DotsAndBoxesBoard::applyMove(const Move& main_move)
{
    const DotsAndBoxesMove& move = static_cast<const DotsAndBoxesMove&>(main_move);
    int row = move.getRow();
    int column = move.getColumn();
    bool isHorizontal;

    if (move.getDirection() == lineDirection::HORIZONTAL)
        isHorizontal = true;
    else
        isHorizontal = false;

    if (isHorizontal) {
        if (horizontal_lines[row][column])
            return;

        horizontal_lines[row][column] = true;
    }
    else {
        if (vertical_lines[row][column])
            return;

        vertical_lines[row][column] = true;
    }
}

bool DotsAndBoxesBoard::isLineTaken(int row, int column, int direction_type) const
{
    if (direction_type == horizontalDirection)
        return horizontal_lines[row][column];

    return vertical_lines[row][column];
}

bool DotsAndBoxesBoard::checkAndCloseBoxes(int row, int column, int direction_type, int player_id)
{
    bool boxClosed = false;

    if (direction_type == horizontalDirection) {
        if (row > 0 && horizontal_lines[row - 1][column] && vertical_lines[row - 1][column] && vertical_lines[row - 1][column + 1])
            boxClosed = closeBox(row - 1, column, player_id) || boxClosed;

        if (row < board_size && horizontal_lines[row + 1][column] && vertical_lines[row][column] && vertical_lines[row][column + 1])
            boxClosed = closeBox(row, column, player_id) || boxClosed;
    }
    else {
        if (column > 0 && vertical_lines[row][column - 1] && horizontal_lines[row][column - 1] && horizontal_lines[row + 1][column - 1])
            boxClosed = closeBox(row, column - 1, player_id) || boxClosed;

        if (column < board_size && vertical_lines[row][column + 1] && horizontal_lines[row][column] && horizontal_lines[row + 1][column])
            boxClosed = closeBox(row, column, player_id) || boxClosed;
    }

    return boxClosed;
}

bool DotsAndBoxesBoard::isFull() const
{
    for (const QVector<int> &row : captured_boxes)
        for (int playerId : row)
            if (playerId == 0)
                return false;

    return true;
}

int DotsAndBoxesBoard::getBoardSize() const
{
    return board_size;
}

const QVector<QVector<int>> &DotsAndBoxesBoard::getCapturedBoxes() const
{
    return captured_boxes;
}

const QVector<QVector<bool>> &DotsAndBoxesBoard::getHorizontalLines() const
{
    return horizontal_lines;
}

const QVector<QVector<bool>> &DotsAndBoxesBoard::getVerticalLines() const
{
    return vertical_lines;
}

bool DotsAndBoxesBoard::closeBox(int row, int column, int player_id)
{
    if (captured_boxes[row][column] != 0)
        return false;

    captured_boxes[row][column] = player_id;
    return true;
}