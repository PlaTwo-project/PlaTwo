#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UI/Auth/Login/login.h"
#include "UI/Auth/SignUp/signup.h"
#include "UI/Auth/ForgotPassword/forgot_password.h"
#include "UI/Menu/main_menu.h"
#include "UI/Menu/Proflie/edit_profile.h"
#include "UI/Menu/game_menu.h"
#include "UI/Network/guest_page.h"
#include "UI/Network/host_page.h"
#include "UI/Menu/history.h"
#include "UI/Games/DotsAndBoxesPage/dots_and_boxes_page.h"
#include "UI/Games/NineMensMorrisPage/nine_mens_morris_page.h"
#include "UI/Games/Fanorona/fanorona_page.h"
#include "UI/Chat/chat_widget.h"
#include <QStackedWidget>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //create pages
    login_page = new Login(this);
    signup_page = new Signup(this);
    forgot_password_page = new ForgotPassword(this);
    main_menu_page = new MainMenu(this);
    edit_profile_page = new EditProfile(this);
    game_menu_page = new GameMenu(this);
    host_page = new HostPage(this);
    guest_page = new GuestPage(this);
    history_page = new History(this);
    dots_and_boxes_page = new DotsAndBoxesPage(this);
    nine_mens_morris_page = new NineMensMorrisPage(this);
    fanorona_page = new FanoronaPage(this);

    // chat
    game_stack = new QStackedWidget(this);
    game_stack->addWidget(dots_and_boxes_page);
    game_stack->addWidget(nine_mens_morris_page);
    game_stack->addWidget(fanorona_page);

    chat_widget = new ChatWidget(this);
    game_room_page = new QWidget(this);
    QHBoxLayout* game_room_layout = new QHBoxLayout(game_room_page);
    game_room_layout->setContentsMargins(0, 0, 0, 0);
    game_room_layout->setSpacing(0);
    game_room_layout->addWidget(game_stack, 1);
    game_room_layout->addWidget(chat_widget, 0);
    connect(chat_widget, &ChatWidget::messageSubmitted, this, &MainWindow::chatMessageSendRequested);

    // add pages
    ui->stackedWidget->addWidget(login_page);
    ui->stackedWidget->addWidget(signup_page);
    ui->stackedWidget->addWidget(forgot_password_page);
    ui->stackedWidget->addWidget(main_menu_page);
    ui->stackedWidget->addWidget(edit_profile_page);
    ui->stackedWidget->addWidget(game_menu_page);
    ui->stackedWidget->addWidget(host_page);
    ui->stackedWidget->addWidget(guest_page);
    ui->stackedWidget->addWidget(history_page);
    ui->stackedWidget->addWidget(game_room_page);

    // received signals
    connect(login_page, &Login::navigateToSignup, this, [this](QString username, QString password) {
        signup_page->setInitialValues(username, password);
        showSignupPage();
    });

    connect(login_page, &Login::navigateToForgotPassword, this, [this](QString username) {
        forgot_password_page->setInitialValues(username);
        showForgotPasswordPage();
    });

    connect(signup_page, &Signup::navigateToLogin, this, &MainWindow::showLoginPage);

    connect(forgot_password_page, &ForgotPassword::navigateToLogin, this, &MainWindow::showLoginPage);

    connect(main_menu_page, &MainMenu::navigateToGame1, this, &MainWindow::showGameMenuPage);

    connect(main_menu_page, &MainMenu::navigateToGame2, this, &MainWindow::showGameMenuPage);

    connect(main_menu_page, &MainMenu::navigateToGame3, this, &MainWindow::showGameMenuPage);

    connect(main_menu_page, &MainMenu::navigateToEditProfile, this, &MainWindow::showEditProfilePage);

    connect(main_menu_page, &MainMenu::navigateToLogin, this, &MainWindow::showLoginPage);

    connect(edit_profile_page, &EditProfile::navigateToMainMenu, this, &MainWindow::showMainMenuPage);

    connect(game_menu_page, &GameMenu::navigateToMainMenu, this, &MainWindow::showMainMenuPage);

    connect(game_menu_page, &GameMenu::navigateToHostPage, this, &MainWindow::showHostPage);

    connect(game_menu_page, &GameMenu::navigateToGuestPage, this, &MainWindow::showGuestPage);

    connect(history_page, &History::navigateToGameMenu, this, [this]() {
        showGameMenuPage(cur_game);
    });

    connect(host_page, &HostPage::navigateToGameMenu, this, [this]() {
        showGameMenuPage(cur_game);
    });

    connect(guest_page, &GuestPage::navigateToGameMenu, this, [this]() {
        showGameMenuPage(cur_game);
    });

    // Mediator signals
    connect(login_page, &Login::loginRequested, this, &MainWindow::loginRequested);
    connect(signup_page, &Signup::signupRequested, this, &MainWindow::signupRequested);
    connect(forgot_password_page, &ForgotPassword::forgotPasswordStep2Requested, this, &MainWindow::forgotPasswordStep2Requested);
    connect(forgot_password_page, &ForgotPassword::resetPasswordRequested, this, &MainWindow::resetPasswordRequested);

    connect(edit_profile_page, &EditProfile::editProfileRequested, this, &MainWindow::editProfileRequested);
    connect(game_menu_page, &GameMenu::navigateToHistory, this, &MainWindow::showHistoryRequested);

    connect(host_page, &HostPage::createRoomRequested, this, [this](int port, int board_size, int time_limit, int color_index) {
        emit createRoomRequested(port, board_size, time_limit, cur_game, color_index);
    });
    connect(guest_page, &GuestPage::joinRequested, this, [this](const QString& IP, const int& port, int color_index) {
        emit joinRoomRequested(IP, port, cur_game, color_index);
    });
    connect(host_page, &HostPage::cancelHostRequested, this, &MainWindow::cancelHostRequested);

    connect(dots_and_boxes_page, &DotsAndBoxesPage::moveRequested, this, &MainWindow::dotsAndBoxesMoveRequested);
    connect(nine_mens_morris_page, &NineMensMorrisPage::moveRequested, this, &MainWindow::nineMensMorrisMoveRequested);
    connect(fanorona_page, &FanoronaPage::moveRequested, this, &MainWindow::fanoronaMoveRequested);

    connect(dots_and_boxes_page, &BasePage::resignRequested, this, &MainWindow::resignRequested);
    connect(nine_mens_morris_page, &BasePage::resignRequested, this, &MainWindow::resignRequested);
    connect(fanorona_page, &BasePage::resignRequested, this, &MainWindow::resignRequested);

    connect(dots_and_boxes_page, &BasePage::pauseRequested, this, &MainWindow::pauseRequested);
    connect(nine_mens_morris_page, &BasePage::pauseRequested, this, &MainWindow::pauseRequested);
    connect(fanorona_page, &BasePage::pauseRequested, this, &MainWindow::pauseRequested);

    // show pages
    showLoginPage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showLoginPage()
{
    ui->stackedWidget->setCurrentWidget(login_page);
}

void MainWindow::showSignupPage()
{
    ui->stackedWidget->setCurrentWidget(signup_page);
}

void MainWindow::showForgotPasswordPage()
{
    forgot_password_page->switchToVerifyPage();
    ui->stackedWidget->setCurrentWidget(forgot_password_page);
}

void MainWindow::showForgotPasswordPage2(const QString& username, const QString &phone)
{
    forgot_password_page->setUserData(username, phone);
    forgot_password_page->switchToResetPage();
}

void MainWindow::showMainMenuPage()
{
    ui->stackedWidget->setCurrentWidget(main_menu_page);
}

void MainWindow::showEditProfilePage()
{
    ui->stackedWidget->setCurrentWidget(edit_profile_page);
}

void MainWindow::showGameMenuPage(const GameName game_name)
{
    cur_game = game_name;
    game_menu_page->setTitle(game_name);
    ui->stackedWidget->setCurrentWidget(game_menu_page);
}

void MainWindow::showHostPage(const GameName game_name)
{
    host_page->setPage(game_name);
    ui->stackedWidget->setCurrentWidget(host_page);
}

void MainWindow::showWatingHostPage(const QString& ip)
{
    host_page->switchToWaitingStatus(ip);
}

void MainWindow::showGuestPage(const GameName game_name)
{
    guest_page->setTitle(game_name);
    ui->stackedWidget->setCurrentWidget(guest_page);
}

void MainWindow::showHistoryPage(const QList<MatchRecord>& historyList, int currentUserId, GameName game_name)
{
    history_page->setHistory(historyList, currentUserId, game_name);
    ui->stackedWidget->setCurrentWidget(history_page);
}

void MainWindow::showDotsAndBoxesPage(const int size) {
    dots_and_boxes_page->setupBoard(size);
    game_stack->setCurrentWidget(dots_and_boxes_page);
    ui->stackedWidget->setCurrentWidget(game_room_page);
}

void MainWindow::showNineMensMorrisPage() {
    nine_mens_morris_page->setupBoard(0);
    game_stack->setCurrentWidget(nine_mens_morris_page);
    ui->stackedWidget->setCurrentWidget(game_room_page);
}

void MainWindow::showFanoronaPage() {
    fanorona_page->setupBoard(0);
    game_stack->setCurrentWidget(fanorona_page);
    ui->stackedWidget->setCurrentWidget(game_room_page);
}

void MainWindow::renderActivePage(const Game* game) {
    BasePage* active_page = qobject_cast<BasePage*>(game_stack->currentWidget());
    if (active_page) {
        active_page->updateFromGame(game);
    }
}

void MainWindow::loadUserDataInProfile(const QString& name, const QString& username, const QString& email, const QString& phone) {
    edit_profile_page->setInitialValues(name, username, email, phone);
}

void MainWindow::updateScoresAndTurn(const int score1, const int score2, const QString& turn_text, const bool is_my_turn) {
    BasePage* active_page = qobject_cast<BasePage*>(game_stack->currentWidget());
    if (active_page) {
        active_page->updateScores(score1, score2);
        active_page->setTurnInfo(turn_text, is_my_turn);
    }
}

void MainWindow::setPlayerNames(const QString& name1, const QString& name2) {
    BasePage* active_page = qobject_cast<BasePage*>(game_stack->currentWidget());
    if (active_page)
        active_page->setPlayerNames(name1, name2);
}

BasePage* MainWindow::getActivePage() const {
    return qobject_cast<BasePage*>(game_stack->currentWidget());
}

void MainWindow::clearLoginFields() {
    login_page->clearFields();
}

void MainWindow::clearSignupFields() {
    signup_page->clearFields();
}

void MainWindow::clearFPFields() {
    forgot_password_page->clearFields();
}

void MainWindow::appendOwnChatMessage(const QString& text) {
    chat_widget->appendMessage(QString(), text, true);
}

void MainWindow::receiveChatMessage(const QString& sender_name, const QString& text) {
    chat_widget->appendMessage(sender_name, text, false);
}

void MainWindow::clearChat() {
    chat_widget->clearMessages();
}

void MainWindow::setDotsAndBoxesColors(const QColor& host_color, const QColor& guest_color) {
    dots_and_boxes_page->setPlayerColors(host_color, guest_color);
}