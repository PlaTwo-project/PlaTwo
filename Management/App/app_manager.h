#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include <QObject>
#include "UI/MainWindow/mainwindow.h"
#include "Management/Auth/authenticator.h"
#include "Infrastructure/DataBase/storage_manager.h"
#include "Infrastructure/DataBase/history_storage_manager.h"

class AppManager : public QObject
{
    Q_OBJECT

public:
    explicit AppManager(QObject *parent = nullptr);
    void run();

private:
    MainWindow* main_window;
    StorageManager userStorage;
    Authenticator authenticator;
    HistoryStorageManager historyStorage;

    void setupConnections();

private slots:
    void handleLogin(const QString &username, const QString &password);
    void handleSignup(const QString &name, const QString &username, const QString &email, const QString &phone, const QString &password);
    void handleForgotPasswordStep2(const QString &username, const QString &phone);
    void handleResetPassword(const QString &username, const QString &phone, const QString &new_password);
    void handleEditProfile(const QString &name, const QString &username, const QString &email, const QString &phone, const QString &old_password, const QString &new_password);
};

#endif // APP_MANAGER_H