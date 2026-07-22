#include "dots_and_boxes_board.h"
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesMove/dots_and_boxes_move.h"

DotsAndBoxesBoard::DotsAndBoxesBoard(int size) : board_size(size) {
    initialize();
}

void DotsAndBoxesBoard::initialize() {
    horizontal_lines.assign(board_size + 1, QVector<int>(board_size, 0));
    vertical_lines.assign(board_size, QVector<int>(board_size + 1, 0));
    captured_boxes.assign(board_size, QVector<int>(board_size, 0));
}

void DotsAndBoxesBoard::clear() {
    initialize();
}

void DotsAndBoxesBoard::applyMove(const Move& main_move) {
    applyMove(main_move, 0);
}

void DotsAndBoxesBoard::applyMove(const Move& main_move, int player_id) {
    const DotsAndBoxesMove& move = static_cast<const DotsAndBoxesMove&>(main_move);
    int row = move.getRow();
    int column = move.getColumn();
    bool isHorizontal = (move.getDirection() == lineDirection::HORIZONTAL);
    if (isHorizontal) {
        if (horizontal_lines[row][column] != 0)
            return;

        horizontal_lines[row][column] = player_id;
    }
    else {
        if (vertical_lines[row][column] != 0)
            return;

        vertical_lines[row][column] = player_id;
    }
}

bool DotsAndBoxesBoard::isLineTaken(int row, int column, int direction_type) const {
    if (direction_type == horizontalDirection)
        return horizontal_lines[row][column] != 0;

    return vertical_lines[row][column] != 0;
}

bool DotsAndBoxesBoard::checkAndCloseBoxes(int row, int column, int direction_type, int player_id) {
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

bool DotsAndBoxesBoard::isFull() const {
    for (const QVector<int> &row : captured_boxes)
        for (int playerId : row)
            if (playerId == 0)
                return false;

    return true;
}

int DotsAndBoxesBoard::getBoardSize() const {
    return board_size;
}

const QVector<QVector<int>> &DotsAndBoxesBoard::getCapturedBoxes() const {
    return captured_boxes;
}

const QVector<QVector<int>> &DotsAndBoxesBoard::getHorizontalLines() const {
    return horizontal_lines;
}

const QVector<QVector<int>> &DotsAndBoxesBoard::getVerticalLines() const {
    return vertical_lines;
}

bool DotsAndBoxesBoard::closeBox(int row, int column, int player_id) {
    if (captured_boxes[row][column] != 0)
        return false;

    captured_boxes[row][column] = player_id;
    return true;
}

void DotsAndBoxesBoard::restoreState(const QVector<QVector<int>>& h_lines, const QVector<QVector<int>>& v_lines, const QVector<QVector<int>>& boxes) {
    horizontal_lines = h_lines;
    vertical_lines = v_lines;
    captured_boxes = boxes;
}