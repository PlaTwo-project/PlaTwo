#include "dots_and_boxes_board.h"
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesMove/dots_and_boxes_move.h"

DotsAndBoxesBoard::DotsAndBoxesBoard(int size) : board_size(size) {
    initialize();
}

void DotsAndBoxesBoard::initialize() {
    horizontal_lines = QVector<QVector<PlayerSlot>>(board_size + 1, QVector<PlayerSlot>(board_size, PlayerSlot::NONE));
    vertical_lines = QVector<QVector<PlayerSlot>>(board_size, QVector<PlayerSlot>(board_size + 1, PlayerSlot::NONE));
    captured_boxes = QVector<QVector<PlayerSlot>>(board_size, QVector<PlayerSlot>(board_size, PlayerSlot::NONE));
}

void DotsAndBoxesBoard::clear() {
    initialize();
}

void DotsAndBoxesBoard::applyMove(const Move& main_move) {
    applyMove(main_move, PlayerSlot::NONE);
}

void DotsAndBoxesBoard::applyMove(const Move& main_move, PlayerSlot player_id) {
    const DotsAndBoxesMove& move = static_cast<const DotsAndBoxesMove&>(main_move);
    int row = move.getRow();
    int column = move.getColumn();
    bool isHorizontal = (move.getDirection() == lineDirection::HORIZONTAL);
    if (isHorizontal) {
        if (horizontal_lines[row][column] != PlayerSlot::NONE)
            return;

        horizontal_lines[row][column] = player_id;
    } else {
        if (vertical_lines[row][column] != PlayerSlot::NONE)
            return;

        vertical_lines[row][column] = player_id;
    }
}

bool DotsAndBoxesBoard::isLineTaken(int row, int column, int direction_type) const {
    if (direction_type == horizontalDirection)
        return horizontal_lines[row][column] != PlayerSlot::NONE;

    return vertical_lines[row][column] != PlayerSlot::NONE;
}

int DotsAndBoxesBoard::checkAndCloseBoxes(int row, int column, int direction_type, PlayerSlot player_id) {
    int boxClosed = 0;
    if (direction_type == horizontalDirection) {
        if (row > 0 && static_cast<int>(horizontal_lines[row - 1][column]) && static_cast<int>(vertical_lines[row - 1][column]) && static_cast<int>(vertical_lines[row - 1][column + 1]))
            if (closeBox(row - 1, column, player_id))
                boxClosed++;

        if (row < board_size && static_cast<int>(horizontal_lines[row + 1][column]) && static_cast<int>(vertical_lines[row][column]) && static_cast<int>(vertical_lines[row][column + 1]))
            if (closeBox(row, column, player_id))
                boxClosed++;
    }
    else {
        if (column > 0 && static_cast<int>(vertical_lines[row][column - 1]) && static_cast<int>(horizontal_lines[row][column - 1]) && static_cast<int>(horizontal_lines[row + 1][column - 1]))
            if (closeBox(row, column - 1, player_id))
                boxClosed++;

        if (column < board_size && static_cast<int>(vertical_lines[row][column + 1]) && static_cast<int>(horizontal_lines[row][column]) && static_cast<int>(horizontal_lines[row + 1][column]))
            if (closeBox(row, column, player_id))
                boxClosed++;
    }

    return boxClosed;
}

bool DotsAndBoxesBoard::isFull() const {
    for (const QVector<PlayerSlot> &row : captured_boxes)
        for (PlayerSlot playerId : row)
            if (playerId == PlayerSlot::NONE)
                return false;

    return true;
}

int DotsAndBoxesBoard::getBoardSize() const {
    return board_size;
}

const QVector<QVector<PlayerSlot>> &DotsAndBoxesBoard::getCapturedBoxes() const {
    return captured_boxes;
}

const QVector<QVector<PlayerSlot>> &DotsAndBoxesBoard::getHorizontalLines() const {
    return horizontal_lines;
}

const QVector<QVector<PlayerSlot>> &DotsAndBoxesBoard::getVerticalLines() const {
    return vertical_lines;
}

bool DotsAndBoxesBoard::closeBox(int row, int column, PlayerSlot player_id) {
    if (captured_boxes[row][column] != PlayerSlot::NONE)
        return false;

    captured_boxes[row][column] = player_id;
    return true;
}

void DotsAndBoxesBoard::restoreState(const QVector<QVector<PlayerSlot>>& h_lines, const QVector<QVector<PlayerSlot>>& v_lines, const QVector<QVector<PlayerSlot>>& boxes) {
    horizontal_lines = h_lines;
    vertical_lines = v_lines;
    captured_boxes = boxes;
}