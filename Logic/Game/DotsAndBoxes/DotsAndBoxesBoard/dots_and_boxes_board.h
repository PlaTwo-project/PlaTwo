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
    void applyMove(const Move& main_move, int player_id);
    bool isLineTaken(int row, int column, int direction_type) const;
    int checkAndCloseBoxes(int row, int column, int direction_type, int player_id);
    bool isFull() const;

    int getBoardSize() const;
    const QVector<QVector<int>> &getCapturedBoxes() const;
    const QVector<QVector<int>> &getVerticalLines() const;
    const QVector<QVector<int>> &getHorizontalLines() const;
    void restoreState(const QVector<QVector<int>>& h_lines, const QVector<QVector<int>>& v_lines, const QVector<QVector<int>>& boxes);

private:
    static const int horizontalDirection = 0;
    static const int verticalDirection   = 1;

    bool closeBox(int row, int column, int player_id);

    int board_size;
    QVector<QVector<int>> horizontal_lines;
    QVector<QVector<int>> vertical_lines;
    QVector<QVector<int>>  captured_boxes;
};