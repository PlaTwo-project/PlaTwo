#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Logic/Game/game_name.h"

class MatchRecord;
class Login;
class Signup;
class ForgotPassword;
class ForgotPassword2;
class MainMenu;
class EditProfile;
class GameMenu;
class HostPage;
class GuestPage;
class History;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showLoginPage();
    void showSignupPage();
    void showForgotPasswordPage();
    void showForgotPasswordPage2(const QString& username, const QString& phone);
    void showMainMenuPage();
    void showEditProfilePage();
    void showGameMenuPage(const GameName game_name);
    void showHostPage(const GameName game_name);
    void showWatingHostPage(const QString& ip);
    void showGuestPage(const GameName game_name);
    void showHistoryPage(const QList<MatchRecord>& historyList, int currentUserId, GameName game_name);

    void loadUserDataInProfile(const QString& name, const QString& username, const QString& email, const QString& phone);

    void clearLoginFields();
    void clearSignupFields();
    void clearFPFields();
    void clearFP2Fields();

signals:
    void loginRequested(const QString& username, const QString& password);
    void signupRequested(const QString& name, const QString& username, const QString& email, const QString& phone, const QString& password);
    void forgotPasswordStep2Requested(const QString& username, const QString& phone);
    void resetPasswordRequested(const QString& username, const QString& phone, const QString& newPassword);
    void editProfileRequested(const QString& name, const QString& username, const QString& email, const QString& phone, const QString& new_password, const QString& old_password);\
    void showHistoryRequested(GameName game_name);
    void createRoomRequested(int port, int board_size, int time_limit, GameName game_name);
    void joinRoomRequested(const QString& IP, const int& port, GameName game_name);
    void cancelHostRequested();

private:
    Ui::MainWindow *ui;

    GameName cur_game;

    Login* login_page;
    Signup* signup_page;
    ForgotPassword* forgot_password_page;
    ForgotPassword2* forgot_password_page2;
    MainMenu* main_menu_page;
    EditProfile* edit_profile_page;
    GameMenu* game_menu_page;
    HostPage* host_page;
    GuestPage* guest_page;
    History* history_page;
};

#endif // MAINWINDOW_H