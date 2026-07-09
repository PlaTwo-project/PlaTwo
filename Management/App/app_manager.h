#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include <QObject>
#include "UI/MainWindow/mainwindow.h"
#include "Management/Auth/authenticator.h"
#include "Infrastructure/DataBase/history_storage_manager.h"
#include "Management/Game/game_manager.h"

class AppManager : public QObject
{
    Q_OBJECT

public:
    explicit AppManager(QObject *parent = nullptr);
    void run();

private:
    MainWindow* main_window;
    Authenticator authenticator;
    HistoryStorageManager history_storage;
    GameManager game_manager;

    void setupConnections();

private slots:
    void handleLogin(const QString &username, const QString &password);
    void handleSignup(const QString &name, const QString &username, const QString &email, const QString &phone, const QString &password);
    void handleForgotPasswordStep2(const QString &username, const QString &phone);
    void handleResetPassword(const QString &username, const QString &phone, const QString &new_password);
    void handleEditProfile(const QString &name, const QString &username, const QString &email, const QString &phone, const QString &old_password, const QString &new_password);
    void handleShowHistory(GameName game_name);
    void handleCreateRoom(int port, int board_size, int time_limit, GameName game_name);
    void handleJoinRoom(const QString& IP, const QString& port, GameName game_name);
    void handleCancelHost();
};

#endif // APP_MANAGER_H