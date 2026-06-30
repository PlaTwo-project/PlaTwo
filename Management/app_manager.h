#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include <QObject>
#include "UI/mainwindow.h"
#include "Management/authenticator.h"

class AppManager : public QObject
{
    Q_OBJECT

public:
    explicit AppManager(QObject *parent = nullptr);
    void run();

private:
    MainWindow* mainWindow;
    Authenticator authenticator;

    void setupConnections();

private slots:
    void handleLogin(const QString& username, const QString& password);
    void handleSignup(const QString& name, const QString& username, const QString& email, const QString& phone, const QString& password);
    void handleForgotPasswordStep2(const QString& username, const QString& phone);
    void handleResetPassword(const QString& username, const QString& phone, const QString& newPassword);
};

#endif
 // APP_MANAGER_H