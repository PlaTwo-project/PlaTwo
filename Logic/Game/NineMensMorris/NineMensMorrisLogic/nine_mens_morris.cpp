#include "nine_mens_morris.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

NineMensMorris::NineMensMorris(const User &player_one, const User &player_two)
    : first_player(player_one), second_player(player_two), first_player_score(0), second_player_score(0), awaiting_removal(false)
{
    game_board = new NineMensMorrisBoard();
    placed_count[0] = 0;
    placed_count[1] = 0;
    current_player = first_player;
}

NineMensMorris::~NineMensMorris()
{
    delete game_board;
}

bool NineMensMorris::makeMove(const Move &main_move)
{
    if (!isValidMove(main_move))
        return false;

    const NineMensMorrisMove &move = static_cast<const NineMensMorrisMove &>(main_move);
    int mover = move.getPlayerId();
    game_board->applyMove(main_move);

    if (move.getMoveType() == MoveType::REMOVE)
    {
        if (mover == 1)
            first_player_score++;
        else
            second_player_score++;

        awaiting_removal = false;
        switchTurn();
        return true;
    }
    else if (move.getMoveType() == MoveType::PLACE)
        placed_count[mover - 1]++;

    if (game_board->isMill(move.getToPosition(), mover))
        awaiting_removal = true;
    else
        switchTurn();

    return true;
}

GameStatus NineMensMorris::checkWin()
{
    if (awaiting_removal)
        return GameStatus::ONGOING;

    if (placed_count[0] == PIECES_PER_PLAYER && game_board->getPieceCount(1) < 3)
        return GameStatus::GUEST_WIN;

    if (placed_count[1] == PIECES_PER_PLAYER && game_board->getPieceCount(2) < 3)
        return GameStatus::HOST_WIN;

    int mover = getCurrentPlayerId();
    bool mover_placing = placed_count[mover - 1] < PIECES_PER_PLAYER;

    if (!mover_placing)
        if (!game_board->hasAnyMove(mover, isFlying(mover)))
        {
            if (mover == 1)
                return GameStatus::GUEST_WIN;
            else
                return GameStatus::HOST_WIN;
        }

    return GameStatus::ONGOING;
}

void NineMensMorris::resetGame()
{
    first_player_score = 0;
    second_player_score = 0;
    placed_count[0] = 0;
    placed_count[1] = 0;
    awaiting_removal = false;
    game_board->clear();
    current_player = first_player;
}

QString NineMensMorris::serializeState() const {

    QJsonObject stateObj;
    stateObj["current_player_id"] = current_player.getId();
    stateObj["first_player_score"] = first_player_score;
    stateObj["second_player_score"] = second_player_score;
    stateObj["placed_count_0"] = placed_count[0];
    stateObj["placed_count_1"] = placed_count[1];
    stateObj["awaiting_removal"] = awaiting_removal;

    QJsonArray boardArray;
    NineMensMorrisBoard* nmm_board = static_cast<NineMensMorrisBoard*>(game_board);
    for (int owner : nmm_board->getPositionOwners())
        boardArray.append(owner);

    stateObj["board_state"] = boardArray;

    return QJsonDocument(stateObj).toJson(QJsonDocument::Compact);
}

void NineMensMorris::loadState(const QString &state_data) {
    QJsonDocument doc = QJsonDocument::fromJson(state_data.toUtf8());
    if (!doc.isObject()) return;
    QJsonObject stateObj = doc.object();

    int current_id = stateObj["current_player_id"].toInt();
    current_player = (current_id == first_player.getId()) ? first_player : second_player;

    first_player_score = stateObj["first_player_score"].toInt();
    second_player_score = stateObj["second_player_score"].toInt();
    placed_count[0] = stateObj["placed_count_0"].toInt();
    placed_count[1] = stateObj["placed_count_1"].toInt();
    awaiting_removal = stateObj["awaiting_removal"].toBool();

    QJsonArray boardArray = stateObj["board_state"].toArray();
    QVector<int> owners;
    for (const QJsonValue& val : boardArray)
        owners.append(val.toInt());

    NineMensMorrisBoard* nmm_board = static_cast<NineMensMorrisBoard*>(game_board);
    nmm_board->setPositionOwners(owners);
}

bool NineMensMorris::isValidMove(const Move &main_move)
{
    const NineMensMorrisMove &move = static_cast<const NineMensMorrisMove &>(main_move);
    int mover = move.getPlayerId();

    if (mover != getCurrentPlayerId())
        return false;

    if (awaiting_removal)
    {
        if (move.getMoveType() != MoveType::REMOVE)
            return false;

        return game_board->isPieceRemovable(move.getToPosition(), getOpponentId(mover));
    }
    else if (move.getMoveType() == MoveType::REMOVE)
        return false;

    bool placing_phase = placed_count[mover - 1] < PIECES_PER_PLAYER;
    if (placing_phase)
    {
        if (move.getMoveType() != MoveType::PLACE)
            return false;

        return game_board->isEmpty(move.getToPosition());
    }
    else if (move.getMoveType() != MoveType::MOVE)
        return false;

    int from = move.getFromPosition();
    int to = move.getToPosition();

    if (game_board->getPositionOwner(from) != mover || !game_board->isEmpty(to))
        return false;

    if (isFlying(mover))
        return true;

    return game_board->isProximate(from, to);
}

int NineMensMorris::getFirstPlayerScore() const
{
    return first_player_score;
}

int NineMensMorris::getSecondPlayerScore() const
{
    return second_player_score;
}

Board *NineMensMorris::getBoard() const
{
    return game_board;
}

bool NineMensMorris::getAwaitingRemoval() const
{
    return awaiting_removal;
}

int NineMensMorris::getPlacedCount(int player_id) const
{
    return placed_count[player_id - 1];
}

bool NineMensMorris::isFlying(int player_id) const
{
    return game_board->getPieceCount(player_id) == FLYING_THRESHOLD;
}

int NineMensMorris::getCurrentPlayerId() const
{
    if (current_player.getId() == first_player.getId())
        return 1;
    else
        return 2;
}

int NineMensMorris::getOpponentId(int player_id) const
{
    if (player_id == 1)
        return 2;
    else
        return 1;
}

void NineMensMorris::switchTurn()
{
    if (getCurrentPlayerId() == 1)
        current_player = second_player;
    else
        current_player = first_player;
}