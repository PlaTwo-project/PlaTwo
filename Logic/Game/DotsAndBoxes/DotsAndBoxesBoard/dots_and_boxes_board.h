#pragma once

#include "Logic/Game/BaseLogicClasses/Board.h"
#include "Logic/Game/BaseLogicClasses/Move.h"
#include "Logic/Constants/player_slot.h"
#include <QVector>

class DotsAndBoxesBoard : public Board
{
public:
    DotsAndBoxesBoard(int size);
    ~DotsAndBoxesBoard() override = default;

    void initialize() override;
    void clear() override;
    void applyMove(const Move& main_move) override;
    void applyMove(const Move& main_move, PlayerSlot player);
    bool isLineTaken(int row, int column, int direction_type) const;
    int checkAndCloseBoxes(int row, int column, int direction_type, PlayerSlot player_id);
    bool isFull() const;

    int getBoardSize() const;
    const QVector<QVector<PlayerSlot>> &getCapturedBoxes() const;
    const QVector<QVector<PlayerSlot>> &getVerticalLines() const;
    const QVector<QVector<PlayerSlot>> &getHorizontalLines() const;
    void restoreState(const QVector<QVector<PlayerSlot>>& h_lines, const QVector<QVector<PlayerSlot>>& v_lines, const QVector<QVector<PlayerSlot>>& boxes);

private:
    static const int horizontalDirection = 0;
    static const int verticalDirection = 1;

    bool closeBox(int row, int column, PlayerSlot player_id);

    int board_size;
    QVector<QVector<PlayerSlot>> horizontal_lines;
    QVector<QVector<PlayerSlot>> vertical_lines;
    QVector<QVector<PlayerSlot>>  captured_boxes;
};