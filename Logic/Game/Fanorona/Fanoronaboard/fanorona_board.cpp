#include "fanorona_board.h"
#include "Logic/Game/Fanorona/FanoronaMove/fanorona_move.h"

FanoronaBoard::FanoronaBoard() {
    initialize();
}

void FanoronaBoard::initialize() {
    occupants.fill(PlayerSlot::NONE, TOTAL_POSITIONS);
    for (int col = 0; col < COLS; ++col) {
        occupants[positionOf(0, col)] = PlayerSlot::GUEST;
        occupants[positionOf(1, col)] = PlayerSlot::GUEST;
        occupants[positionOf(3, col)] = PlayerSlot::HOST;
        occupants[positionOf(4, col)] = PlayerSlot::HOST;
    }

    for (int col = 0; col < COLS; ++col) {
        int centre_col = COLS / 2;
        if (col == centre_col)
            occupants[positionOf(2, col)] = PlayerSlot::NONE;
        else if (col < centre_col) {
            if (col % 2 == 0)
                occupants[positionOf(2, col)] = PlayerSlot::GUEST;
            else
                occupants[positionOf(2, col)] = PlayerSlot::HOST;
        } else {
            if (col % 2 == 0)
                occupants[positionOf(2, col)] = PlayerSlot::HOST;
            else
                occupants[positionOf(2, col)] = PlayerSlot::GUEST;
        }
    }
}

void FanoronaBoard::clear() {
    initialize();
}

void FanoronaBoard::applyMove(const Move& main_move) {
    const FanoronaMove* move = dynamic_cast<const FanoronaMove*>(&main_move);
    if (!move)
        return;

    int from = move->getFrom();
    int to = move->getTo();
    QVector<int> captured;
    if (move->getCaptureType() != FanoronaCaptureType::NONE)
        captured = getCaptureTargets(from, to, move->getCaptureType());

    occupants[to] = occupants[from];
    occupants[from] = PlayerSlot::NONE;

    for (int position : captured)
        occupants[position] = PlayerSlot::NONE;
}

bool FanoronaBoard::isEmpty(int position) const {
    return occupants[position] == PlayerSlot::NONE;
}

PlayerSlot FanoronaBoard::getOccupant(int position) const {
    return occupants[position];
}

int FanoronaBoard::rowOf(int position) {
    return position / COLS;
}

int FanoronaBoard::colOf(int position) {
    return position % COLS;
}

int FanoronaBoard::positionOf(int row, int col) {
    return row * COLS + col;
}

bool FanoronaBoard::isStrongPoint(int position) {
    return (rowOf(position) + colOf(position)) % 2 == 0;
}

bool FanoronaBoard::isAdjacent(int from, int to) const {
    int dr = rowOf(to) - rowOf(from);
    int dc = colOf(to) - colOf(from);
    if (dr < -1 || dr > 1 || dc < -1 || dc > 1)
        return false;

    bool is_diagonal = (dr != 0 && dc != 0);
    if (is_diagonal)
        return isStrongPoint(from);

    return true;
}

QVector<int> FanoronaBoard::getNeighbours(int position) const {
    QVector<int> neighbours;
    int r = rowOf(position);
    int c = colOf(position);
    bool strong = isStrongPoint(position);

    static const int dr8[8] = { -1, -1, -1,  0, 0,  1, 1, 1 };
    static const int dc8[8] = { -1,  0,  1, -1, 1, -1, 0, 1 };
    static const int dr4[4] = { -1, 1,  0, 0 };
    static const int dc4[4] = {  0, 0, -1, 1 };
    const int* dr = strong ? dr8 : dr4;
    const int* dc = strong ? dc8 : dc4;

    int dir_count;
    if (strong)
        dir_count = 8;
    else
        dir_count = 4;

    for (int i = 0; i < dir_count; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS)
            neighbours.append(positionOf(nr, nc));
    }

    return neighbours;
}

QVector<int> FanoronaBoard::getCaptureTargets(int from, int to, FanoronaCaptureType type) const {
    QVector<int> result;
    if (type == FanoronaCaptureType::NONE)
        return result;

    PlayerSlot mover = occupants[from];
    if (mover == PlayerSlot::NONE)
        return result;

    PlayerSlot opponent;
    if (mover == PlayerSlot::HOST)
        opponent = PlayerSlot::GUEST;
    else
        opponent = PlayerSlot::HOST;

    int dr = rowOf(to) - rowOf(from);
    int dc = colOf(to) - colOf(from);
    int cursor_row, cursor_col, inc_r, inc_c;

    if (type == FanoronaCaptureType::APPROACH) {
        cursor_row = rowOf(to) + dr;
        cursor_col = colOf(to) + dc;
        inc_r = dr;
        inc_c = dc;
    } else {
        cursor_row = rowOf(from) - dr;
        cursor_col = colOf(from) - dc;
        inc_r = -dr;
        inc_c = -dc;
    }

    while (cursor_row >= 0 && cursor_row < ROWS && cursor_col >= 0 && cursor_col < COLS) {
        int position = positionOf(cursor_row, cursor_col);
        if (occupants[position] != opponent)
            break;

        result.append(position);
        cursor_row += inc_r;
        cursor_col += inc_c;
    }

    return result;
}

bool FanoronaBoard::canApproachCapture(int from, int to) const {
    return !getCaptureTargets(from, to, FanoronaCaptureType::APPROACH).isEmpty();
}

bool FanoronaBoard::canWithdrawalCapture(int from, int to) const {
    return !getCaptureTargets(from, to, FanoronaCaptureType::WITHDRAWAL).isEmpty();
}

int FanoronaBoard::getPieceCount(PlayerSlot player_id) const {
    int count = 0;
    for (PlayerSlot occupant : occupants)
        if (occupant == player_id)
            ++count;

    return count;
}

bool FanoronaBoard::hasAnyCaptureAvailable(PlayerSlot player_id) const {
    for (int position = 0; position < TOTAL_POSITIONS; ++position) {
        if (occupants[position] != player_id)
            continue;

        for (int neighbour : getNeighbours(position)) {
            if (!isEmpty(neighbour))
                continue;

            if (canApproachCapture(position, neighbour) || canWithdrawalCapture(position, neighbour)) 
                return true;
        }
    }

    return false;
}

bool FanoronaBoard::hasAnyLegalMove(PlayerSlot player_id) const {
    for (int position = 0; position < TOTAL_POSITIONS; ++position) {
        if (occupants[position] != player_id)
            continue;

        for (int neighbour : getNeighbours(position))
            if (isEmpty(neighbour))
                return true;
    }

    return false;
}

const QVector<PlayerSlot>& FanoronaBoard::getOccupants() const {
    return occupants;
}

void FanoronaBoard::setOccupants(const QVector<PlayerSlot>& new_occupants) {
    if (new_occupants.size() == TOTAL_POSITIONS)
        occupants = new_occupants;
}