#include "fanorona.h"
#include "Logic/Game/Fanorona/FanoronaMove/fanorona_move.h"
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

Fanorona::Fanorona(const User &player_one, const User &player_two)
    : first_player(player_one), second_player(player_two), first_player_score(0), second_player_score(0)
{
    game_board = new FanoronaBoard();
    resetChainState();
    current_player = first_player;
}

Fanorona::~Fanorona()
{
    delete game_board;
}

int Fanorona::idOf(const User &user) const
{
    return user.getId() == first_player.getId() ? 1 : 2;
}

int Fanorona::currentPlayerId() const
{
    return idOf(current_player);
}

int Fanorona::opponentId(int player_id) const
{
    return player_id == 1 ? 2 : 1;
}

void Fanorona::switchTurn()
{
    current_player = (currentPlayerId() == 1) ? second_player : first_player;
}

void Fanorona::resetChainState()
{
    chain_active = false;
    chain_position = -1;
    chain_visited.clear();
    chain_last_dr = 0;
    chain_last_dc = 0;
}

bool Fanorona::isChainActive() const
{
    return chain_active;
}

int Fanorona::getChainPosition() const
{
    return chain_position;
}

bool Fanorona::mustCapture() const
{
    return game_board->hasAnyCaptureAvailable(currentPlayerId());
}

FanoronaCaptureType Fanorona::resolveCaptureType(int from, int to, FanoronaCaptureType requested, bool &is_ambiguous) const
{
    is_ambiguous = false;

    bool can_approach = game_board->canApproachCapture(from, to);
    bool can_withdrawal = game_board->canWithdrawalCapture(from, to);

    if (can_approach && can_withdrawal)
    {
        is_ambiguous = true;
        if (requested == FanoronaCaptureType::APPROACH || requested == FanoronaCaptureType::WITHDRAWAL)
            return requested;

        return FanoronaCaptureType::NONE;
    }

    if (can_approach)
        return FanoronaCaptureType::APPROACH;

    if (can_withdrawal)
        return FanoronaCaptureType::WITHDRAWAL;

    return FanoronaCaptureType::NONE;
}

bool Fanorona::hasContinuation(int position, int incoming_dr, int incoming_dc, const QVector<int> &visited) const
{
    for (int neighbour : game_board->getNeighbours(position))
    {
        if (!game_board->isEmpty(neighbour))
            continue;

        if (visited.contains(neighbour))
            continue;

        int dr = FanoronaBoard::rowOf(neighbour) - FanoronaBoard::rowOf(position);
        int dc = FanoronaBoard::colOf(neighbour) - FanoronaBoard::colOf(position);

        if (dr == incoming_dr && dc == incoming_dc)
            continue;

        if (game_board->canApproachCapture(position, neighbour) || game_board->canWithdrawalCapture(position, neighbour))
            return true;
    }

    return false;
}

bool Fanorona::isValidMove(const Move &main_move)
{
    const auto &move = static_cast<const FanoronaMove &>(main_move);
    int mover = move.getPlayerId();

    if (mover != 1 && mover != 2)
        return false;

    if (mover != currentPlayerId())
        return false;

    if (move.isEndTurn())
        return chain_active;

    int from = move.getFrom();
    int to = move.getTo();

    if (!FanoronaBoard::isValidPosition(from) || !FanoronaBoard::isValidPosition(to))
        return false;

    if (game_board->getOccupant(from) != mover)
        return false;

    if (!game_board->isEmpty(to))
        return false;

    if (!game_board->isAdjacent(from, to))
        return false;

    if (chain_active)
    {
        if (from != chain_position)
            return false;

        if (chain_visited.contains(to))
            return false;

        int dr = FanoronaBoard::rowOf(to) - FanoronaBoard::rowOf(from);
        int dc = FanoronaBoard::colOf(to) - FanoronaBoard::colOf(from);

        if (dr == chain_last_dr && dc == chain_last_dc)
            return false;

        bool can_approach = game_board->canApproachCapture(from, to);
        bool can_withdrawal = game_board->canWithdrawalCapture(from, to);

        if (!can_approach && !can_withdrawal)
            return false;

        if (can_approach && can_withdrawal)
        {
            if (move.getCaptureType() != FanoronaCaptureType::APPROACH &&
                move.getCaptureType() != FanoronaCaptureType::WITHDRAWAL)
                return false;
        }

        return true;
    }

    bool can_approach = game_board->canApproachCapture(from, to);
    bool can_withdrawal = game_board->canWithdrawalCapture(from, to);

    if (can_approach && can_withdrawal)
    {
        if (move.getCaptureType() != FanoronaCaptureType::APPROACH &&
            move.getCaptureType() != FanoronaCaptureType::WITHDRAWAL)
            return false;
    }

    bool this_move_captures = can_approach || can_withdrawal;

    if (!this_move_captures && game_board->hasAnyCaptureAvailable(mover))
        return false;

    return true;
}

bool Fanorona::makeMove(const Move &main_move)
{
    if (!isValidMove(main_move))
        return false;

    const auto &move = static_cast<const FanoronaMove &>(main_move);
    int mover = move.getPlayerId();

    if (move.isEndTurn())
    {
        resetChainState();
        switchTurn();
        return true;
    }

    int from = move.getFrom();
    int to = move.getTo();

    int dr = FanoronaBoard::rowOf(to) - FanoronaBoard::rowOf(from);
    int dc = FanoronaBoard::colOf(to) - FanoronaBoard::colOf(from);

    bool ambiguous;
    FanoronaCaptureType chosen = resolveCaptureType(from, to, move.getCaptureType(), ambiguous);

    QVector<int> captured;
    if (chosen != FanoronaCaptureType::NONE)
        captured = game_board->getCaptureTargets(from, to, chosen);

    FanoronaMove effective_move(from, to, mover, chosen, false);
    game_board->applyMove(effective_move);

    if (!captured.isEmpty())
    {
        if (mover == 1)
            first_player_score += captured.size();
        else
            second_player_score += captured.size();
    }

    if (chosen != FanoronaCaptureType::NONE)
    {
        QVector<int> visited = chain_active ? chain_visited : QVector<int>{from};
        visited.append(to);

        if (hasContinuation(to, dr, dc, visited))
        {
            chain_active = true;
            chain_position = to;
            chain_visited = visited;
            chain_last_dr = dr;
            chain_last_dc = dc;
        }
        else
        {
            resetChainState();
            switchTurn();
        }
    }
    else
    {
        resetChainState();
        switchTurn();
    }

    return true;
}

GameStatus Fanorona::checkWin()
{
    if (chain_active)
        return GameStatus::ONGOING;

    int p1_count = game_board->getPieceCount(1);
    int p2_count = game_board->getPieceCount(2);

    if (p1_count == 0)
        return GameStatus::GUEST_WIN;

    if (p2_count == 0)
        return GameStatus::HOST_WIN;

    int mover = currentPlayerId();
    if (!game_board->hasAnyLegalMove(mover))
        return (mover == 1) ? GameStatus::GUEST_WIN : GameStatus::HOST_WIN;

    return GameStatus::ONGOING;
}

void Fanorona::resetGame()
{
    first_player_score = 0;
    second_player_score = 0;
    game_board->clear();
    resetChainState();
    current_player = first_player;
}

QString Fanorona::serializeState() const {
    QJsonObject state_object;

    state_object["current_player_id"] = current_player.getId();
    state_object["first_player_score"] = first_player_score;
    state_object["second_player_score"] = second_player_score;

    state_object["chain_active"] = chain_active;
    state_object["chain_position"] = chain_position;
    state_object["chain_last_dr"] = chain_last_dr;
    state_object["chain_last_dc"] = chain_last_dc;

    QJsonArray serialized_chain_visited;
    for (int visited_position : chain_visited) {
        serialized_chain_visited.append(visited_position);
    }
    state_object["chain_visited"] = serialized_chain_visited;

    FanoronaBoard* fanorona_board = static_cast<FanoronaBoard*>(game_board);
    QJsonArray serialized_occupants;
    QVector<int> board_occupants = fanorona_board->getOccupants();
    for (int occupant_value : board_occupants) {
        serialized_occupants.append(occupant_value);
    }
    state_object["occupants"] = serialized_occupants;

    return QJsonDocument(state_object).toJson(QJsonDocument::Compact);
}

void Fanorona::loadState(const QString &state_data) {
    QJsonDocument json_document = QJsonDocument::fromJson(state_data.toUtf8());
    if (!json_document.isObject()) return;

    QJsonObject state_object = json_document.object();

    int current_player_id = state_object["current_player_id"].toInt();
    current_player = (current_player_id == first_player.getId()) ? first_player : second_player;
    first_player_score = state_object["first_player_score"].toInt();
    second_player_score = state_object["second_player_score"].toInt();

    chain_active = state_object["chain_active"].toBool();
    chain_position = state_object["chain_position"].toInt();
    chain_last_dr = state_object["chain_last_dr"].toInt();
    chain_last_dc = state_object["chain_last_dc"].toInt();

    chain_visited.clear();
    QJsonArray serialized_chain_visited = state_object["chain_visited"].toArray();
    for (const QJsonValue& visited_position_value : serialized_chain_visited) {
        chain_visited.append(visited_position_value.toInt());
    }

    QJsonArray serialized_occupants = state_object["occupants"].toArray();
    QVector<int> board_occupants;
    for (const QJsonValue& occupant_value : serialized_occupants) {
        board_occupants.append(occupant_value.toInt());
    }

    FanoronaBoard* fanorona_board = static_cast<FanoronaBoard*>(game_board);
    if (board_occupants.size() == FanoronaBoard::TOTAL_POSITIONS) {
        fanorona_board->setOccupants(board_occupants);
    }
}


int Fanorona::getFirstPlayerScore() const
{
    return first_player_score;
}

int Fanorona::getSecondPlayerScore() const
{
    return second_player_score;
}

Board *Fanorona::getBoard() const
{
    return game_board;
}