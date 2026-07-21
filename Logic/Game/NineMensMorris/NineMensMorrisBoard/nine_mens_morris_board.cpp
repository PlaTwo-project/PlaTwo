#include "nine_mens_morris_board.h"

NineMensMorrisBoard::NineMensMorrisBoard() {
    initialize();
}

QVector<QVector<int>> NineMensMorrisBoard::buildProximityList() {
    static const int edges[][2] = {
        {0, 1}, {1, 2}, {2, 14}, {14, 23}, {23, 22}, {22, 21}, {21, 9}, {9, 0},
        {3, 4}, {4, 5}, {5, 13}, {13, 20}, {20, 19}, {19, 18}, {18, 10}, {10, 3},
        {6, 7}, {7, 8}, {8, 12}, {12, 17}, {17, 16}, {16, 15}, {15, 11}, {11, 6},
        {1, 4}, {4, 7}, {14, 13}, {13, 12}, {22, 19}, {19, 16}, {9, 10}, {10, 11}
    };

    QVector<QVector<int>> proximate(NineMensMorrisBoard::TOTAL_POSITIONS);
    for (const auto& edge : edges) {
        proximate[edge[0]].append(edge[1]);
        proximate[edge[1]].append(edge[0]);
    }

    return proximate;
}

QVector<QVector<int>> NineMensMorrisBoard::buildMillLinesList() {
    return QVector<QVector<int>> {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {9, 10, 11}, {12, 13, 14}, {15, 16, 17}, {18, 19, 20}, {21, 22, 23},
        {0, 9, 21}, {3, 10, 18}, {6, 11, 15}, {1, 4, 7}, {16, 19, 22}, {8, 12, 17}, {5, 13, 20}, {2, 14, 23}
    };
}

QVector<QPair<int, int>> NineMensMorrisBoard::buildCoordinates() {
    return QVector<QPair<int, int>> {
        {0, 0}, {3, 0}, {6, 0},
        {1, 1}, {3, 1}, {5, 1},
        {2, 2}, {3, 2}, {4, 2},
        {0, 3}, {1, 3}, {2, 3}, {4, 3}, {5, 3}, {6, 3},
        {2, 4}, {3, 4}, {4, 4},
        {1, 5}, {3, 5}, {5, 5},
        {0, 6}, {3, 6}, {6, 6}
    };
}

void NineMensMorrisBoard::initialize() {
    position_owners.assign(TOTAL_POSITIONS, 0);
}

void NineMensMorrisBoard::clear() {
    initialize();
}

void NineMensMorrisBoard::applyMove(const Move& main_move) {
    const NineMensMorrisMove& move = static_cast<const NineMensMorrisMove&>(main_move);
    switch (move.getMoveType()) {
    case MoveType::PLACE:
        position_owners[move.getToPosition()] = move.getPlayerId();
        break;

    case MoveType::MOVE:
        position_owners[move.getToPosition()] = position_owners[move.getFromPosition()];
        position_owners[move.getFromPosition()] = 0;
        break;

    case MoveType::REMOVE:
        position_owners[move.getToPosition()] = 0;
        break;
    }
}

bool NineMensMorrisBoard::isEmpty(int position) const {
    return position_owners[position] == 0;
}

bool NineMensMorrisBoard::isProximate(int from, int to) const {
    return getProximityList()[from].contains(to);
}

bool NineMensMorrisBoard::isMill(int position, int player_id) const {
    for (const auto& line : getMillLinesList()) {
        if (!line.contains(position))
            continue;

        bool all_owned = true;
        for (int p : line) {
            if (position_owners[p] != player_id) {
                all_owned = false;
                break;
            }
        }
        if (all_owned)
            return true;
    }

    return false;
}

bool NineMensMorrisBoard::areAllPiecesInMills(int player_id) const
{
    for (int p = 0; p < TOTAL_POSITIONS; p++) {
        if (position_owners[p] != player_id)
            continue;

        if (!isMill(p, player_id))
            return false;
    }

    return true;
}

bool NineMensMorrisBoard::isPieceRemovable(int position, int owner_id) const {
    if (position_owners[position] != owner_id)
        return false;

    if (!isMill(position, owner_id))
        return true;

    return areAllPiecesInMills(owner_id);
}

bool NineMensMorrisBoard::hasAnyMove(int player_id, bool is_flying) const {
    if (is_flying)
        return true;

    for (int p = 0; p < TOTAL_POSITIONS; p++) {
        if (position_owners[p] != player_id)
            continue;

        if (!is_flying)
            for (int neighbour : getProximityList()[p])
                if (isEmpty(neighbour))
                    return true;
    }

    return false;
}

int NineMensMorrisBoard::getPositionOwner(int position) const {
    return position_owners[position];
}

void NineMensMorrisBoard::setPositionOwners(const QVector<int>& owners) {
    position_owners = owners;
}

int NineMensMorrisBoard::getPieceCount(int player_id) const {
    int count = 0;
    for (int owner : position_owners)
        if (owner == player_id)
            count ++;

    return count;
}

const QVector<int>& NineMensMorrisBoard::getPositionOwners() const {
    return position_owners;
}

const QVector<QVector<int>>& NineMensMorrisBoard::getProximityList() {
    static const auto list = buildProximityList();
    return list;
}

const QVector<QVector<int>>& NineMensMorrisBoard::getMillLinesList() {
    static const auto list = buildMillLinesList();
    return list;
}

QPair<int, int> NineMensMorrisBoard::getCoordinates(int position) {
    if (position < 0 || position >= TOTAL_POSITIONS)
        return QPair<int, int> (0, 0);

    static const auto list = buildCoordinates();
    return list[position];
}