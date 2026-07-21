#ifndef NINE_MENS_MORRIS_BOARD_H
#define NINE_MENS_MORRIS_BOARD_H

#include <QVector>
#include <QPair>
#include "Logic/Game/BaseLogicClasses/Board.h"
#include "Logic/Game/NineMensMorris/NineMensMorrisMove/nine_mens_morris_move.h"

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
    bool isMill(int position, int player_id) const;
    bool areAllPiecesInMills(int player_id) const;
    bool isPieceRemovable(int position, int owner_id) const;
    bool hasAnyMove(int player_id, bool is_flying) const;

    int getPositionOwner(int position) const;
    void setPositionOwners(const QVector<int>& owners);
    int getPieceCount(int player_id) const;
    const QVector<int>& getPositionOwners() const;
    static const QVector<QVector<int>>& getProximityList();
    static const QVector<QVector<int>>& getMillLinesList();
    static QPair<int, int> getCoordinates(int position);

    static const int TOTAL_POSITIONS = 24;

private:
    QVector<int> position_owners; // 0 = empty, 1 = player one, 2 = player two

    static QVector<QVector<int>> buildProximityList();
    static QVector<QVector<int>> buildMillLinesList();
    static QVector<QPair<int, int>> buildCoordinates();
};

#endif // NINE_MENS_MORRIS_BOARD_H