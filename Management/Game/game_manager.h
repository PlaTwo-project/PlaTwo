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

    QString createRoom(const User& host_user, const int port, const GameName game_name, const int board_size, const int time_limit);
    bool joinRoom(const User& guest_user, const QString& host_ip, const int port, const GameName game_name);
    bool cancelRoom();

    void startGame();
    bool handleLocalMove(int arg1, int arg2, int arg3);
    bool handleRemoteMove(const QByteArray& serialized_move);
    void handleGuestConnection(const User& guest_user);
    void handleRoomConfigReceived(const User& host_user, int board_size, int time_limit);
    void handleTimeLimitReached();

    Role getRole() const;
    Game* getCurrentGame() const;
    int getRoomBoardSize() const;
    GameName getGameName() const;

    void updateGuestUser(const User& guest_user);
    void updateRoomConfig(const User& host_user, int board_size, int time_limit);

signals:
    void gameStarted();
    void moveAppliedSuccessfully(bool is_turn_kept);
    void opponentMoveReceived();
    void gameOver(GameStatus status);
    void gameTimeUp();

private:
    Role role;
    RoomState* room_state;
    Host* host;
    Guest* guest;
    Game* current_game;
    QTimer* time_limit_timer;
    QElapsedTimer game_duration_timer;
};

#endif // GAMEMANAGER_H