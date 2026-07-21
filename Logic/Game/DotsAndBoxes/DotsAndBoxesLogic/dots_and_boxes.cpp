#include "dots_and_boxes.h"
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesMove/dots_and_boxes_move.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

DotsAndBoxes::DotsAndBoxes(int size, const User &player_one, const User &player_two)
    : first_player(player_one), second_player(player_two), first_player_score(0), second_player_score(0), grid_size(size)
{
    game_board = new DotsAndBoxesBoard(grid_size);
    current_player = first_player;
}

DotsAndBoxes::~DotsAndBoxes()
{
    delete game_board;
}

bool DotsAndBoxes::isValidMove(const Move &main_move)
{
    const DotsAndBoxesMove &move = static_cast<const DotsAndBoxesMove &>(main_move);
    return !game_board->isLineTaken(move.getRow(), move.getColumn(), static_cast<int>(move.getDirection()));
}

bool DotsAndBoxes::makeMove(const Move &main_move)
{
    if (!isValidMove(main_move))
        return false;

    game_board->applyMove(main_move);
    const DotsAndBoxesMove &move = static_cast<const DotsAndBoxesMove &>(main_move);
    int p_id;
    if (current_player.getId() == first_player.getId())
        p_id = 1;
    else
        p_id = 2;

    bool closed = game_board->checkAndCloseBoxes(move.getRow(), move.getColumn(), static_cast<int>(move.getDirection()), p_id);
    if (closed)
    {
        if (p_id == 1)
            first_player_score++;
        else
            second_player_score++;
    }
    else
    {
        if (current_player.getId() == first_player.getId())
            current_player = second_player;
        else
            current_player = first_player;
    }

    return true;
}

GameStatus DotsAndBoxes::checkWin()
{
    if (!game_board->isFull())
        return GameStatus::ONGOING;

    if (first_player_score > second_player_score)
        return GameStatus::HOST_WIN;

    if (second_player_score > first_player_score)
        return GameStatus::GUEST_WIN;

    return GameStatus::DRAW;
}

void DotsAndBoxes::resetGame()
{
    first_player_score = 0;
    second_player_score = 0;
    game_board->clear();
    current_player = first_player;
}

QString DotsAndBoxes::serializeState() const {

    QJsonObject state_obj;
    state_obj["current_player_id"] = current_player.getId();
    state_obj["first_player_score"] = first_player_score;
    state_obj["second_player_score"] = second_player_score;

    DotsAndBoxesBoard* db_board = static_cast<DotsAndBoxesBoard*>(game_board);

    auto serializeBoolMatrix = [](const QVector<QVector<bool>>& bool_matrix) {
        QJsonArray serialized_matrix;
        for (const auto& matrix_row : bool_matrix) {
            QJsonArray serialized_row;
            for (bool cell_value : matrix_row)
                serialized_row.append(cell_value);
            serialized_matrix.append(serialized_row);
        }
        return serialized_matrix;
    };

    auto serializeIntMatrix = [](const QVector<QVector<int>>& int_matrix) {
        QJsonArray serialized_matrix;
        for (const auto& matrix_row : int_matrix) {
            QJsonArray serialized_row;
            for (int cell_value : matrix_row)
                serialized_row.append(cell_value);
            serialized_matrix.append(serialized_row);
        }
        return serialized_matrix;
    };

    state_obj["horizontal_lines"] = serializeBoolMatrix(db_board->getHorizontalLines());
    state_obj["vertical_lines"] = serializeBoolMatrix(db_board->getVerticalLines());
    state_obj["captured_boxes"] = serializeIntMatrix(db_board->getCapturedBoxes());

    return QJsonDocument(state_obj).toJson(QJsonDocument::Compact);
}

void DotsAndBoxes::loadState(const QString &state_data) {
    QJsonDocument json_document = QJsonDocument::fromJson(state_data.toUtf8());
    if (!json_document.isObject())
        return;

    QJsonObject state_obj = json_document.object();

    int current_player_id = state_obj["current_player_id"].toInt();
    current_player = (current_player_id == first_player.getId()) ? first_player : second_player;
    first_player_score = state_obj["first_player_score"].toInt();
    second_player_score = state_obj["second_player_score"].toInt();

    auto parseBoolMatrix = [](const QJsonArray& serialized_matrix) {
        QVector<QVector<bool>> bool_matrix;
        for (const QJsonValue& row_json_value : serialized_matrix) {
            QVector<bool> matrix_row;
            for (const QJsonValue& cell_value : row_json_value.toArray()) {
                matrix_row.append(cell_value.toBool());
            }
            bool_matrix.append(matrix_row);
        }
        return bool_matrix;
    };

    auto parseIntMatrix = [](const QJsonArray& serialized_matrix) {
        QVector<QVector<int>> int_matrix;
        for (const QJsonValue& row_json_value : serialized_matrix) {
            QVector<int> matrix_row;
            for (const QJsonValue& cell_value : row_json_value.toArray()) {
                matrix_row.append(cell_value.toInt());
            }
            int_matrix.append(matrix_row);
        }
        return int_matrix;
    };

    QVector<QVector<bool>> horizontal_lines = parseBoolMatrix(state_obj["horizontal_lines"].toArray());
    QVector<QVector<bool>> vertical_lines = parseBoolMatrix(state_obj["vertical_lines"].toArray());
    QVector<QVector<int>> captured_boxes = parseIntMatrix(state_obj["captured_boxes"].toArray());

    DotsAndBoxesBoard* db_board = static_cast<DotsAndBoxesBoard*>(game_board);
    db_board->restoreState(horizontal_lines, vertical_lines, captured_boxes);
}



int DotsAndBoxes::getFirstPlayerScore() const
{
    return first_player_score;
}

int DotsAndBoxes::getSecondPlayerScore() const
{
    return second_player_score;
}

Board *DotsAndBoxes::getBoard() const
{
    return game_board;
}