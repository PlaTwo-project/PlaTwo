#include "Management/Game/game_manager.h"
#include "Infrastructure/DataBase/history_storage_manager.h"
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesLogic/dots_and_boxes.h"
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesMove/dots_and_boxes_move.h"
#include "Logic/Game/Fanorona/FanoronaLogic/fanorona.h"
#include "Logic/Game/Fanorona/FanoronaMove/fanorona_move.h"
#include "Logic/Game/NineMensMorris/NineMensMorrisLogic/nine_mens_morris.h"
#include "Logic/Game/NineMensMorris/NineMensMorrisMove/nine_mens_morris_move.h"
#include "Logic/Game/Record/match_record.h"
#include <QDataStream>

GameManager::GameManager(QObject *parent)
    : QObject(parent), room_state(nullptr), host(nullptr), guest(nullptr), current_game(nullptr) {
    time_limit_timer = new QTimer(this);
    time_limit_timer->setSingleShot(true);
    connect(time_limit_timer, &QTimer::timeout, this, &GameManager::handleTimeLimitReached);
}

GameManager::~GameManager() {
    delete room_state;
    delete current_game;
}

QString GameManager::createRoom(const User& host_user, const int port, const GameName game_name, const int board_size, const int time_limit, const int host_color_index) {
    role = Role::Host;
    host = new Host(this);
    QString local_ip = host->getLocalIP();
    room_state = new RoomState(port, host_user, User(), game_name, local_ip, board_size, time_limit);
    room_state->setHostColorIndex(host_color_index);
    host->startHosting(port);

    connect(host, &Host::guestJoined, this, &GameManager::handleGuestConnection);
    connect(host, &Host::moveReceived, this, &GameManager::handleRemoteMove);
    connect(host, &Host::resignReceived, this, &GameManager::handleRemoteResign);
    connect(host, &Host::chatMessageReceived, this, [this](const QString& message) {
        emit chatMessageReceived(room_state->getGuestUser().getUsername(), message);
    });
    connect(host, &Network::pauseRequested, this, &GameManager::opponentPauseRequested);
    connect(host, &Network::pauseResponded, this, &GameManager::handleRemotePauseResponse);

    return local_ip;
}

bool GameManager::joinRoom(const User& guest_user, const QString& host_ip, const int port, const GameName game_name, const int guest_color_index) {
    role = Role::Guest;
    guest = new Guest(this);
    room_state = new RoomState(port, User(), guest_user, game_name, host_ip, 0, 0);
    room_state->setGuestColorIndex(guest_color_index);
    guest->connectHost(host_ip, port);

    connect(guest, &Network::connected, this, [this, guest_user, guest_color_index]() { guest->sendGuestInfo(guest_user, guest_color_index);});
    connect(guest, &Guest::roomConfigReceived, this, &GameManager::handleRoomConfigReceived);
    connect(guest, &Guest::moveReceived, this, &GameManager::handleRemoteMove);
    connect(guest, &Guest::resignReceived, this, &GameManager::handleRemoteResign);
    connect(guest, &Guest::chatMessageReceived, this, [this](const QString& message) {
        emit chatMessageReceived(room_state->getHostUser().getUsername(), message);
    });
    connect(guest, &Network::pauseRequested, this, &GameManager::opponentPauseRequested);
    connect(guest, &Network::pauseResponded, this, &GameManager::handleRemotePauseResponse);

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
    accumulated_time = 0;

    if (room_state->getGameName() == GameName::DotsAndBoxes)
        current_game = new DotsAndBoxes(room_state->getBoardSize(), room_state->getHostUser(), room_state->getGuestUser());
    else if (room_state->getGameName() == GameName::NineMensMorris)
        current_game = new NineMensMorris(room_state->getHostUser(), room_state->getGuestUser());
    else if (room_state->getGameName() == GameName::Fanorona)
        current_game = new Fanorona(room_state->getHostUser(), room_state->getGuestUser());

    SavedGameStorageManager storage;
    SavedGame saved_record;
    if (storage.findSavedGame(room_state->getHostUser().getId(), room_state->getGuestUser().getId(), room_state->getGameName(), saved_record)) {
        current_game->loadState(saved_record.getStateData());
        accumulated_time = saved_record.getElapsedTime(); 
    }

    game_duration_timer.start();

    int limit_minutes = room_state->getTimeLimit();
    if (limit_minutes > 0) {
        int remaining_time = (limit_minutes * 60 * 1000) - (accumulated_time * 1000);
        if (remaining_time < 0) remaining_time = 0;
        time_limit_timer->start(remaining_time);
    }
    else
        time_limit_timer->stop();

    emit gameStarted();
}

bool GameManager::handleLocalMove(int arg1, int arg2, int arg3) {
    if (!current_game)
        return false;

    Move* proposed_move = nullptr;
    if (room_state->getGameName() == GameName::DotsAndBoxes)
        proposed_move = new DotsAndBoxesMove(arg1, arg2, static_cast<lineDirection>(arg3));
    else if (room_state->getGameName() == GameName::NineMensMorris) {
        int mover_id = (current_game->getCurrentPlayer().getId() == room_state->getHostUser().getId()) ? 1 : 2;
        auto move_type = static_cast<MoveType>(arg3);
        int from = (move_type == MoveType::MOVE) ? arg1 : -1;
        int to = (move_type == MoveType::MOVE) ? arg2 : arg1;
        proposed_move = new NineMensMorrisMove(move_type, mover_id, from, to);
    }
    else if (room_state->getGameName() == GameName::Fanorona) {
        int mover_id = (current_game->getCurrentPlayer().getId() == room_state->getHostUser().getId()) ? 1 : 2;

        if (arg1 == -1 && arg2 == -1 && arg3 == -1) {
            proposed_move = new FanoronaMove(-1, -1, mover_id, FanoronaCaptureType::NONE, true);
        }
        else {
            FanoronaCaptureType requested_capture = FanoronaCaptureType::NONE;
            if (arg3 == 0)
                requested_capture = FanoronaCaptureType::APPROACH;
            else if (arg3 == 1)
                requested_capture = FanoronaCaptureType::WITHDRAWAL;

            proposed_move = new FanoronaMove(arg1, arg2, mover_id, requested_capture, false);
        }
    }
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
        if (status != GameStatus::ONGOING){
            if (time_limit_timer->isActive())
                time_limit_timer->stop();
            if (room_state)
                room_state->setDuration(game_duration_timer.elapsed() / 1000);

            saveMatchRecord(status);
            emit gameOver(status);
        }
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
    else if (room_state->getGameName() == GameName::NineMensMorris) {
        QString move_string = QString::fromUtf8(serialized_move);
        QStringList parts = move_string.split(',');
        if (parts.size() < 4) return false;
        remote_move = new NineMensMorrisMove(static_cast<MoveType>(parts[0].toInt()),parts[1].toInt(), parts[2].toInt(), parts[3].toInt());
    }
    else if (room_state->getGameName() == GameName::Fanorona) {
        QString move_string = QString::fromUtf8(serialized_move);
        QStringList parts = move_string.split(',');

        if (parts.size() < 5)
            return false;

        remote_move = new FanoronaMove(parts[0].toInt(), parts[1].toInt(), parts[2].toInt(), static_cast<FanoronaCaptureType>(parts[3].toInt()), parts[4].toInt() != 0);
    }


    if (!remote_move)
        return false;

    bool success = current_game->makeMove(*remote_move);

    if (success) {
        GameStatus status = current_game->checkWin();
        if (status != GameStatus::ONGOING) {
            if (time_limit_timer->isActive())
                time_limit_timer->stop();
            if (room_state)
                room_state->setDuration(game_duration_timer.elapsed() / 1000);

            saveMatchRecord(status);
            emit gameOver(status);
        }
        else
            emit opponentMoveReceived();
    }

    delete remote_move;
    return success;
}

void GameManager::sendChatMessage(const QString& message) {
    if (role == Role::Host)
        host->sendChatMessage(message);
    else if (role == Role::Guest)
        guest->sendChatMessage(message);
}

Role GameManager::getRole() const {
    return role;
}

Game* GameManager::getCurrentGame() const {
    return current_game;
}

int GameManager::getRoomBoardSize() const {
    return room_state->getBoardSize();
}

GameName GameManager::getGameName() const {
    return room_state->getGameName();
}

QString GameManager::getHostUsername() const {
    return room_state->getHostUser().getUsername();
}

QString GameManager::getGuestUsername() const {
    return room_state->getGuestUser().getUsername();
}

int GameManager::getHostUserId() const {
    return room_state->getHostUser().getId();
}

int GameManager::getGuestUserId() const {
    return room_state->getGuestUser().getId();
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

void GameManager::handleGuestConnection(const User& guest_user, int guest_color_index) {
    updateGuestUser(guest_user);
    int host_color_index = room_state->getHostColorIndex();
    int final_guest_color_index = guest_color_index;

    if (room_state->getGameName() == GameName::DotsAndBoxes) {
        int palette_size = DotsAndBoxesColors::palette().size();

        if (final_guest_color_index == host_color_index)
            final_guest_color_index = (host_color_index + 1) % palette_size;
    }

    room_state->setGuestColorIndex(final_guest_color_index);
    host->sendRoomConfig(room_state->getHostUser(), room_state->getBoardSize(), room_state->getTimeLimit(), host_color_index, final_guest_color_index);
    startGame();
}

void GameManager::handleRoomConfigReceived(const User& host_user, int board_size, int time_limit, int host_color_index, int guest_color_index) {
    updateRoomConfig(host_user, board_size, time_limit);
    room_state->setHostColorIndex(host_color_index);
    room_state->setGuestColorIndex(guest_color_index);
    startGame();
}

void GameManager::handleTimeLimitReached() {
    if (!current_game || !room_state) return;

    room_state->setDuration((game_duration_timer.elapsed() / 1000) + accumulated_time);

    int host_score = current_game->getFirstPlayerScore();
    int guest_score = current_game->getSecondPlayerScore();
    GameStatus final_status;

    if (host_score > guest_score) {
        final_status = GameStatus::HOST_WIN;
    }
    else if (guest_score > host_score) {
        final_status = GameStatus::GUEST_WIN;
    }
    else {
        final_status = GameStatus::DRAW;
    }

    saveMatchRecord(final_status);

    emit gameOver(final_status, GameEndReason::TIME_UP);
}

void GameManager::saveMatchRecord(GameStatus status) {

    GameName game_name = room_state->getGameName();
    int host_id = room_state->getHostUser().getId();
    int guest_id = room_state->getGuestUser().getId();
    int host_score = current_game->getFirstPlayerScore();
    int guest_score = current_game->getSecondPlayerScore();
    QString host_username = getHostUsername();
    QString guest_username = getGuestUsername();

    int winner_id = -1;
    if (status == GameStatus::HOST_WIN) {
        winner_id = host_id;
    }
    else if (status == GameStatus::GUEST_WIN) {
        winner_id = guest_id;
    }

    QDateTime current_date = QDateTime::currentDateTime();
    int game_duration = room_state->getDuration();

    MatchRecord new_record(game_name, host_id, host_username, guest_id, guest_username, winner_id, host_score, guest_score, current_date, game_duration);

    history_db.addMatchRecord(new_record);

    SavedGameStorageManager storage;
    storage.removeSavedGame(host_id, guest_id, game_name);
}

void GameManager::handleLocalResign() {
    if (!current_game) return;

    if (time_limit_timer->isActive())
        time_limit_timer->stop();
    if (room_state)
        room_state->setDuration((game_duration_timer.elapsed() / 1000) + accumulated_time);

    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out << qint8(4);

    if (role == Role::Host)
        host->sendData(packet);
    else
        guest->sendData(packet);

    GameStatus final_status = (role == Role::Host) ? GameStatus::GUEST_WIN : GameStatus::HOST_WIN;

    saveMatchRecord(final_status);
    emit gameOver(final_status, GameEndReason::RESIGNATION);
}

void GameManager::handleRemoteResign() {
    if (!current_game) return;

    if (time_limit_timer->isActive())
        time_limit_timer->stop();
    if (room_state)
        room_state->setDuration((game_duration_timer.elapsed() / 1000) + accumulated_time);

    GameStatus final_status = (role == Role::Host) ? GameStatus::HOST_WIN : GameStatus::GUEST_WIN;

    saveMatchRecord(final_status);
    emit gameOver(final_status, GameEndReason::RESIGNATION);
}

void GameManager::requestPause() {
    if (role == Role::Host) 
        host->sendPauseRequest();
    else 
        guest->sendPauseRequest();
}

void GameManager::respondToPause(bool accepted) {
    if (role == Role::Host) 
        host->sendPauseResponse(accepted);
    else 
        guest->sendPauseResponse(accepted);

    if (accepted) 
        executePauseAndSave();
}

void GameManager::handleRemotePauseResponse(bool accepted) {
    emit pauseResponded(accepted);
    if (accepted) 
        executePauseAndSave();
}

void GameManager::executePauseAndSave() {
    if (time_limit_timer->isActive()) 
        time_limit_timer->stop();

    int current_elapsed = (game_duration_timer.elapsed() / 1000) + accumulated_time;

    SavedGameStorageManager storage;
    storage.saveOrUpdateGame(room_state->getGameName(), room_state->getHostUser().getId(), room_state->getGuestUser().getId(),
        room_state->getBoardSize(),room_state->getTimeLimit(), current_elapsed, current_game->serializeState());

    emit gamePausedSuccessfully();
}

QColor GameManager::getHostColor() const {
    return DotsAndBoxesColors::colorAt(room_state ? room_state->getHostColorIndex() : -1);
}

QColor GameManager::getGuestColor() const {
    return DotsAndBoxesColors::colorAt(room_state ? room_state->getGuestColorIndex() : -1);
}