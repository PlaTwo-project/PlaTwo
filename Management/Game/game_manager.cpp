#include "Management/Game/game_manager.h"
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesLogic/dots_and_boxes.h"
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesMove/dots_and_boxes_move.h"

GameManager::GameManager(QObject *parent)
    : QObject(parent), room_state(nullptr), host(nullptr), guest(nullptr), current_game(nullptr) {
}

GameManager::~GameManager() {
    delete room_state;
    delete current_game;
}

QString GameManager::createRoom(const User& host_user, const int port, const GameName game_name, const int board_size, const int time_limit) {
    role = Role::Host;
    host = new Host(this);

    QString local_ip = host->getLocalIP();

    room_state = new RoomState(port, host_user, User(), game_name, local_ip, board_size, time_limit);
    host->startHosting(port);

    connect(host, &Host::guestJoined, this, &GameManager::handleGuestConnection);
    connect(host, &Host::moveReceived, this, &GameManager::handleRemoteMove);

    return local_ip;
}

bool GameManager::joinRoom(const User& guest_user, const QString& host_ip, const int port, const GameName game_name) {
    role = Role::Guest;
    guest = new Guest(this);

    room_state = new RoomState(port, User(), guest_user, game_name, host_ip, 0, 0);
    guest->connectHost(host_ip, port);

    connect(guest, &Network::connected, this, [this, guest_user]() { guest->sendGuestInfo(guest_user);});
    connect(guest, &Guest::roomConfigReceived, this, &GameManager::handleRoomConfigReceived);
    connect(guest, &Guest::moveReceived, this, &GameManager::handleRemoteMove);

    return true;
}

bool GameManager::cancelRoom() {
    if (role != Role::Host)
        return false;

    host->deleteLater();
    host = nullptr;
    delete room_state;
    room_state = nullptr;
    return true;
}

void GameManager::startGame() {
    delete current_game;
    current_game = nullptr;

    if (room_state->getGameName() == GameName::DotsAndBoxes)
        current_game = new DotsAndBoxes(room_state->getBoardSize(), room_state->getHostUser(), room_state->getGuestUser());

    // badan inja else if bazi ha ro ezafe mi conim

    emit gameStarted();
}

bool GameManager::handleLocalMove(int row, int col, int direction) {
    if (!current_game)
        return false;

    Move* proposed_move = nullptr;
    if (room_state->getGameName() == GameName::DotsAndBoxes)
        proposed_move = new DotsAndBoxesMove(row, col, static_cast<lineDirection>(direction));

    User previous_player = current_game->getCurrentPlayer();
    bool success = current_game->makeMove(*proposed_move);

    if (success) {
        QByteArray serialized = proposed_move->serializeMove();

        QByteArray packet;
        QDataStream out(&packet, QIODevice::WriteOnly);
        out << qint8(3) << serialized;

        if (role == Role::Host)
            host->sendData(packet);
        else
            guest->sendData(packet);

        GameStatus status = current_game->checkWin();
        if (status != GameStatus::ONGOING)
            emit gameOver(status);
        else
            emit moveAppliedSuccessfully(previous_player.getId() == current_game->getCurrentPlayer().getId());
    }

    delete proposed_move;
    return success;
}

bool GameManager::handleRemoteMove(const QByteArray& serialized_move) {
    if (!current_game)
        return false;

    Move* remote_move = nullptr;

    if (room_state->getGameName() == GameName::DotsAndBoxes) {
        QString move_string = QString::fromUtf8(serialized_move);
        QStringList parts = move_string.split(',');

        if (parts.size() < 3)
            return false;

        remote_move = new DotsAndBoxesMove(parts[0].toInt(), parts[1].toInt(), static_cast<lineDirection>(parts[2].toInt()));
    }

    if (!remote_move)
        return false;

    bool success = current_game->makeMove(*remote_move);

    if (success) {
        GameStatus status = current_game->checkWin();
        if (status != GameStatus::ONGOING)
            emit gameOver(status);
        else
            emit opponentMoveReceived();
    }

    delete remote_move;
    return success;
}


Role GameManager::getRole() const {
    return role;
}

Game* GameManager::getCurrentGame() const {
    return current_game;
}

int GameManager::getRoomBoardSize() const {
    return room_state ? room_state->getBoardSize() : 8;
}

void GameManager::updateGuestUser(const User& guest_user) {
    if (room_state)
        room_state->setGuestUser(guest_user);
}

void GameManager::updateRoomConfig(const User& host_user, int board_size, int time_limit) {
    if (room_state) {
        room_state->setHostUser(host_user);
        room_state->setBoardSize(board_size);
        room_state->setTimeLimit(time_limit);
    }
}

void GameManager::handleGuestConnection(const User& guest_user) {
    updateGuestUser(guest_user);
    host->sendRoomConfig(room_state->getHostUser(), room_state->getBoardSize(), room_state->getTimeLimit());
    startGame();
}

void GameManager::handleRoomConfigReceived(const User& host_user, int board_size, int time_limit) {
    updateRoomConfig(host_user, board_size, time_limit);
    startGame();
}