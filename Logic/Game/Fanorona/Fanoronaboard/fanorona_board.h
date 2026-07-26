#ifndef FANORONA_BOARD_H
#define FANORONA_BOARD_H

#include "Logic/Game/BaseLogicClasses/Board.h"
#include "Logic/Game/BaseLogicClasses/Move.h"
#include "Logic/Constants/player_slot.h"
#include <QVector>

enum class FanoronaCaptureType
{
    NONE = 0,
    APPROACH = 1,
    WITHDRAWAL = 2
};

class FanoronaBoard : public Board
{
public:
    static const int ROWS = 5;
    static const int COLS = 9;
    static const int TOTAL_POSITIONS = ROWS * COLS;

    FanoronaBoard();
    ~FanoronaBoard() override = default;

    void initialize() override;
    void clear() override;
    void applyMove(const Move& main_move) override;

    bool isEmpty(int position) const;
    PlayerSlot getOccupant(int position) const;

    static int rowOf(int position);
    static int colOf(int position);
    static int positionOf(int row, int col);
    static bool isStrongPoint(int position);

    bool isAdjacent(int from, int to) const;
    QVector<int> getNeighbours(int position) const;

    QVector<int> getCaptureTargets(int from, int to, FanoronaCaptureType type) const;
    bool canApproachCapture(int from, int to) const;
    bool canWithdrawalCapture(int from, int to) const;

    int getPieceCount(PlayerSlot player_id) const;
    bool hasAnyCaptureAvailable(PlayerSlot player_id) const;
    bool hasAnyLegalMove(PlayerSlot player_id) const;

    const QVector<PlayerSlot>& getOccupants() const;
    void setOccupants(const QVector<PlayerSlot>& new_occupants);

private:
    QVector<PlayerSlot> occupants;
};

#endif // FANORONA_BOARD_H