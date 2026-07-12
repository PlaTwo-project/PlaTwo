#include "dots_and_boxes.h"
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesMove/dots_and_boxes_move.h"

DotsAndBoxes::DotsAndBoxes(int size, const User& player_one, const User& player_two)
    : first_player(player_one), second_player(player_two), first_player_score(0), second_player_score(0), grid_size(size) {
    game_board = new DotsAndBoxesBoard(grid_size);
    current_player = first_player;
}

DotsAndBoxes::~DotsAndBoxes() {
    delete game_board;
}

bool DotsAndBoxes::isValidMove(const Move& main_move) {
    const auto& move = static_cast<const DotsAndBoxesMove &>(main_move);
    int row = move.getRow();
    int col = move.getColumn();
    int dir = static_cast<int>(move.getDirection());

    if (row < 0 || col < 0)
        return false;

    if (dir == 0) {
        if (row > grid_size || col >= grid_size)
            return false;
    }
    else {
        if (row >= grid_size || col > grid_size)
            return false;
    }

    return !game_board->isLineTaken(row, col, dir);
}

bool DotsAndBoxes::makeMove(const Move& main_move) {
    if (!isValidMove(main_move))
        return false;

    game_board->applyMove(main_move);
    const auto& move = static_cast<const DotsAndBoxesMove&>(main_move);
    int p_id;
    if (current_player.getId() == first_player.getId())
        p_id = 1;
    else
        p_id = 2;

    bool closed = game_board->checkAndCloseBoxes(move.getRow(), move.getColumn(), static_cast<int>(move.getDirection()), p_id);
    if (closed) {
        if (p_id == 1)
            first_player_score++;
        else
            second_player_score++;
    }
    else {
        if (current_player.getId() == first_player.getId())
            current_player = second_player;
        else
            current_player = first_player;
    }

    return true;
}

GameStatus DotsAndBoxes::checkWin() {
    if (!game_board->isFull())
        return GameStatus::ONGOING;

    if (first_player_score > second_player_score)
        return GameStatus::PLAYER_ONE_WIN;

    if (second_player_score > first_player_score)
        return GameStatus::PLAYER_TWO_WIN;

    return GameStatus::DRAW;
}

void DotsAndBoxes::resetGame() {
    first_player_score = 0;
    second_player_score = 0;
    game_board->clear();
    current_player = first_player;
}

QString DotsAndBoxes::serializeState() const {
    return QString("%1,%2,%3").arg(current_player.getId()).arg(first_player_score).arg(second_player_score);
}

void DotsAndBoxes::loadState(const QString& state_data) {
    QStringList tokens = state_data.split(',');
    if (tokens.size() >= 3) {
        int current_id = tokens[0].toInt();
        first_player_score = tokens[1].toInt();
        second_player_score = tokens[2].toInt();
        if (current_id == first_player.getId())
            current_player = first_player;
        else
            current_player = second_player;
    }
}

int DotsAndBoxes::getFirstPlayerScore() const {
    return first_player_score;
}

int DotsAndBoxes::getSecondPlayerScore() const {
    return second_player_score;
}

Board* DotsAndBoxes::getBoard() const {
    return game_board;
}