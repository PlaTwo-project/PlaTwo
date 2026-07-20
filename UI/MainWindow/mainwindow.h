#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Logic/Game/game_name.h"

class MatchRecord;
class Login;
class Signup;
class ForgotPassword;
class MainMenu;
class EditProfile;
class GameMenu;
class HostPage;
class GuestPage;
class History;
class DotsAndBoxesPage;
class BasePage;
class NineMensMorrisPage;
class FanoronaPage;

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
    void showDotsAndBoxesPage(const int size);
    void showNineMensMorrisPage();
    void showFanoronaPage();
    BasePage* getActivePage() const;
    void renderActivePage(const class Game* game);

    void loadUserDataInProfile(const QString& name, const QString& username, const QString& email, const QString& phone);
    void updateScoresAndTurn(const int score1, const int score2, const QString& turn_text, const bool is_my_turn);
    void setPlayerNames(const QString& name1, const QString& name2);

    void clearLoginFields();
    void clearSignupFields();
    void clearFPFields();

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
    void dotsAndBoxesMoveRequested(const int row, const int col, const int direction);
    void nineMensMorrisMoveRequested(const int from, const int to, const int action_type);
    void fanoronaMoveRequested(const int from, const int to, const int capture_choice);

private:
    Ui::MainWindow *ui;

    GameName cur_game;

    Login* login_page;
    Signup* signup_page;
    ForgotPassword* forgot_password_page;
    MainMenu* main_menu_page;
    EditProfile* edit_profile_page;
    GameMenu* game_menu_page;
    HostPage* host_page;
    GuestPage* guest_page;
    History* history_page;
    DotsAndBoxesPage* dots_and_boxes_page;
    NineMensMorrisPage* nine_mens_morris_page;
    FanoronaPage* fanorona_page;
};

#endif // MAINWINDOW_H