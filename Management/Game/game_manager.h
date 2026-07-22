#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QElapsedTimer>
#include "Logic/Game/room_state.h"
#include "Infrastructure/Network/guest.h"
#include "Infrastructure/Network/host.h"
#include "Logic/Game/BaseLogicClasses/Game.h"
#include "Infrastructure/DataBase/history_storage_manager.h"
#include "Logic/Game/game_end_reason.h"
#include "Infrastructure/DataBase/saved_game_storage_manager.h"
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesColors/dots_and_boxes_colors.h"

enum class Role {
    Host,
    Guest
};

class GameManager : public QObject
{
    Q_OBJECT

public:
    explicit GameManager(QObject *parent = nullptr);
    ~GameManager();

    QString createRoom(const User& host_user, const int port, const GameName game_name, const int board_size, const int time_limit, const int host_color_index = -1);
    bool joinRoom(const User& guest_user, const QString& host_ip, const int port, const GameName game_name, const int guest_color_index = -1);
    bool cancelRoom();

    void startGame();
    bool handleLocalMove(int arg1, int arg2, int arg3);
    bool handleRemoteMove(const QByteArray& serialized_move);
    void handleGuestConnection(const User& guest_user, int guest_color_index);
    void handleRoomConfigReceived(const User& host_user, int board_size, int time_limit, int host_color_index, int guest_color_index);
    void handleTimeLimitReached();
    void handleLocalResign();
    void handleRemoteResign();
    void sendChatMessage(const QString& message);
    void requestPause();
    void respondToPause(bool accepted);
    void handleRemotePauseResponse(bool accepted);
    void executePauseAndSave();

    Role getRole() const;
    Game* getCurrentGame() const;
    int getRoomBoardSize() const;
    GameName getGameName() const;
    QString getHostUsername() const;
    QString getGuestUsername() const;
    int getHostUserId() const;
    int getGuestUserId() const;
    QColor getHostColor() const;
    QColor getGuestColor() const;

    void updateGuestUser(const User& guest_user);
    void updateRoomConfig(const User& host_user, int board_size, int time_limit);
    void saveMatchRecord(GameStatus status);

signals:
    void gameStarted();
    void moveAppliedSuccessfully(bool is_turn_kept);
    void opponentMoveReceived();
    void gameOver(GameStatus status, GameEndReason reason = GameEndReason::LOGIC);
    void chatMessageReceived(const QString& sender_name, const QString& message);
    void opponentPauseRequested();
    void pauseResponded(bool accepted);
    void gamePausedSuccessfully();

private:
    Role role;
    RoomState* room_state;
    Host* host;
    Guest* guest;
    Game* current_game;
    QTimer* time_limit_timer;
    QElapsedTimer game_duration_timer;
    HistoryStorageManager history_db;
    int accumulated_time;
};

#endif // GAMEMANAGER_H