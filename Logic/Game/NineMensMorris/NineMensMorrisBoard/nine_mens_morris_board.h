#ifndef NINE_MENS_MORRIS_BOARD_H
#define NINE_MENS_MORRIS_BOARD_H

#include <QVector>
#include <QPair>
#include "Logic/Game/BaseLogicClasses/Board.h"
#include "Logic/Game/NineMensMorris/NineMensMorrisMove/nine_mens_morris_move.h"
#include "Logic/Constants/player_slot.h"

//        Game Map Hint
//
//  0-----------1-----------2
//  |           |           |
//  |   3-------4-------5   |
//  |   |       |       |   |
//  |   |   6---7---8   |   |
//  |   |   |       |   |   |
//  9---10--11      12--13--14
//  |   |   |       |   |   |
//  |   |   15--16--17  |   |
//  |   |       |       |   |
//  |   18------19------20  |
//  |           |           |
//  21----------22----------23

class NineMensMorrisBoard : public Board
{
public:
    NineMensMorrisBoard();
    ~NineMensMorrisBoard() override = default;

    void initialize() override;
    void clear() override;
    void applyMove(const Move& main_move) override;

    bool isEmpty(int position) const;
    bool isProximate(int from, int to) const;
    bool isMill(int position, PlayerSlot player_id) const;
    bool areAllPiecesInMills(PlayerSlot player_id) const;
    bool isPieceRemovable(int position, PlayerSlot owner_id) const;
    bool hasAnyMove(PlayerSlot player_id, bool is_flying) const;

    PlayerSlot getPositionOwner(int position) const;
    void setPositionOwners(const QVector<PlayerSlot>& owners);
    int getPieceCount(PlayerSlot player_id) const;
    const QVector<PlayerSlot>& getPositionOwners() const;
    static const QVector<QVector<int>>& getProximityList();
    static const QVector<QVector<int>>& getMillLinesList();
    static QPair<int, int> getCoordinates(int position);

    static const int TOTAL_POSITIONS = 24;

private:
    QVector<PlayerSlot> position_owners;

    static QVector<QVector<int>> buildProximityList();
    static QVector<QVector<int>> buildMillLinesList();
    static QVector<QPair<int, int>> buildCoordinates();
};

#endif // NINE_MENS_MORRIS_BOARD_H