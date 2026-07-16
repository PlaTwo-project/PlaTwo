#pragma once

#include "Logic/Game/BaseLogicClasses/Board.h"
#include "Logic/Game/BaseLogicClasses/Move.h"

#include <QVector>

class DotsAndBoxesBoard : public Board
{
public:
    DotsAndBoxesBoard(int size);
    ~DotsAndBoxesBoard() override = default;

    void initialize() override;
    void clear() override;
    void applyMove(const Move& main_move) override;
    bool isLineTaken(int row, int column, int direction_type) const;
    bool checkAndCloseBoxes(int row, int column, int direction_type, int player_id);
    bool isFull() const;

    int getBoardSize() const;
    const QVector<QVector<int>> &getCapturedBoxes() const;
    const QVector<QVector<bool>> &getVerticalLines() const;
    const QVector<QVector<bool>> &getHorizontalLines() const;

private:
    static const int horizontalDirection = 0;
    static const int verticalDirection   = 1;

    bool closeBox(int row, int column, int player_id);

    int board_size;
    QVector<QVector<bool>> horizontal_lines;
    QVector<QVector<bool>> vertical_lines;
    QVector<QVector<int>>  captured_boxes;
};