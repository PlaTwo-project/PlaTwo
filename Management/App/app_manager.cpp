#include "Management/App/app_manager.h"
#include "session_manager.h"
#include "Logic/Game/Record/match_record.h"
#include <QMessageBox>

AppManager::AppManager(QObject *parent)
    : QObject{parent}, authenticator(userStorage), history_storage(), game_manager()
{
    main_window = new MainWindow();
    setupConnections();
}

void AppManager::run()
{
    main_window->show();
}

void AppManager::setupConnections()
{
    connect(main_window, &MainWindow::loginRequested, this, &AppManager::handleLogin);
    connect(main_window, &MainWindow::signupRequested, this, &AppManager::handleSignup);
    connect(main_window, &MainWindow::forgotPasswordStep2Requested, this, &AppManager::handleForgotPasswordStep2);
    connect(main_window, &MainWindow::resetPasswordRequested, this, &AppManager::handleResetPassword);

    connect(main_window, &MainWindow::editProfileRequested, this, &AppManager::handleEditProfile);
    connect(main_window, &MainWindow::showHistoryRequested, this, &AppManager::handleShowHistory);
    connect(main_window, &MainWindow::createRoomRequested, this, &AppManager::handleCreateRoom);
    connect(main_window, &MainWindow::joinRoomRequested, this, &AppManager::handleJoinRoom);
    connect(main_window, &MainWindow::cancelHostRequested, this, &AppManager::handleCancelHost);

    connect(main_window, &MainWindow::dotsAndBoxesMoveRequested, this, &AppManager::handleLocalMove);
    connect(main_window, &MainWindow::nineMensMorrisMoveRequested, this, &AppManager::handleLocalMove);
    connect(main_window, &MainWindow::fanoronaMoveRequested, this, &AppManager::handleLocalMove);
    connect(&game_manager, &GameManager::gameStarted, this, &AppManager::handleGameStarted);
    connect(&game_manager, &GameManager::moveAppliedSuccessfully, this, &AppManager::handleMoveApplied);
    connect(&game_manager, &GameManager::opponentMoveReceived, this, &AppManager::handleOpponentMoveReceived);
    connect(&game_manager, &GameManager::gameOver, this, &AppManager::handleGameOver);
    connect(main_window, &MainWindow::resignRequested, &game_manager, &GameManager::handleLocalResign);
    connect(main_window, &MainWindow::chatMessageSendRequested, this, &AppManager::handleChatMessageSend);
    connect(&game_manager, &GameManager::chatMessageReceived, this, &AppManager::handleChatMessageReceived);

    connect(main_window, &MainWindow::pauseRequested, this, &AppManager::handleLocalPauseRequest);
    connect(&game_manager, &GameManager::opponentPauseRequested, this, &AppManager::handleOpponentPauseRequest);
    connect(&game_manager, &GameManager::pauseResponded, this, &AppManager::handlePauseResponse);
    connect(&game_manager, &GameManager::gamePausedSuccessfully, this, &AppManager::handleGamePausedSuccessfully);
}

void AppManager::handleLogin(const QString &username, const QString &password)
{
    User logged_user;
    AuthResult result = authenticator.login(username, password, logged_user);
    int res;

    switch (result)
    {
    case AuthResult::SUCCESS:
        SessionManager::getInstance().login(logged_user);
        res = QMessageBox::information(main_window, "Login", "Login successfully.");
        if (res == QMessageBox::Ok)
        {
            User user = SessionManager::getInstance().getCurrentUser();
            main_window->loadUserDataInProfile(user.getName(), user.getUsername(), user.getEmail(), user.getPhoneNumber());
            main_window->showMainMenuPage();
            main_window->clearLoginFields();
        }
        break;

    case AuthResult::USER_NOT_FOUND:
        QMessageBox::warning(main_window, "Error", "User not found.");
        break;

    case AuthResult::WRONG_PASSWORD:
        QMessageBox::warning(main_window, "Error", "Wrong password.");
        break;

    default:
        QMessageBox::warning(main_window, "Error", "An unknown error occurred.");
    }
}

void AppManager::handleSignup(const QString &name, const QString &username, const QString &email, const QString &phone, const QString &password)
{
    AuthResult result = authenticator.signup(name, username, email, phone, password);
    int res;

    switch (result)
    {
    case AuthResult::SUCCESS:
        res = QMessageBox::information(main_window, "Signup", "Account created successfully.");
        if (res == QMessageBox::Ok)
        {
            main_window->showLoginPage();
            main_window->clearSignupFields();
        }
        break;

    case AuthResult::USERNAME_TAKEN:
        QMessageBox::warning(main_window, "Error", "Username already taken.");
        break;

    case AuthResult::EMAIL_TAKEN:
        QMessageBox::warning(main_window, "Error", "Email already used.");
        break;

    case AuthResult::PHONE_TAKEN:
        QMessageBox::warning(main_window, "Error", "Phone number already used.");
        break;

    case AuthResult::INVALID_EMAIL:
        QMessageBox::warning(main_window, "Error", "Invalid email.");
        break;

    case AuthResult::INVALID_PHONE:
        QMessageBox::warning(main_window, "Error", "Invalid phone number.");
        break;

    case AuthResult::WEAK_PASSWORD:
        QMessageBox::warning(main_window, "Error", "Weak password.");
        break;

    default:
        QMessageBox::warning(main_window, "Error", "An unknown error occurred.");
    }
}

void AppManager::handleForgotPasswordStep2(const QString &username, const QString &phone)
{
    AuthResult result = authenticator.verifyUserPhone(username, phone);

    switch (result)
    {
    case AuthResult::SUCCESS:
        main_window->showForgotPasswordPage2(username, phone);
        main_window->clearFPFields();
        break;

    case AuthResult::USER_NOT_FOUND:
        QMessageBox::warning(main_window, "Error", "User not found.");
        break;

    case AuthResult::USER_PHONE_MISMATCH:
        QMessageBox::warning(main_window, "Error", "Username and phone do not match.");
        break;

    default:
        QMessageBox::warning(main_window, "Error", "An unknown error occurred.");
    }
}

void AppManager::handleResetPassword(const QString &username, const QString &phone, const QString &new_password)
{
    AuthResult result = authenticator.resetPassword(username, phone, new_password);
    int res;

    switch (result)
    {
    case AuthResult::SUCCESS:
        res = QMessageBox::information(main_window, "Success Message", "Password changed successfully.");
        if (res == QMessageBox::Ok)
        {
            main_window->showLoginPage();
            main_window->clearFPFields();
        }
        break;

    case AuthResult::USER_NOT_FOUND:
        QMessageBox::warning(main_window, "Error", "User not found.");
        break;

    case AuthResult::USER_PHONE_MISMATCH:
        QMessageBox::warning(main_window, "Error", "Username and phone do not match.");
        break;

    case AuthResult::WEAK_PASSWORD:
        QMessageBox::warning(main_window, "Error", "Weak password.");
        break;

    default:
        QMessageBox::warning(main_window, "Error", "An unknown error occurred.");
    }
}

void AppManager::handleEditProfile(const QString &name, const QString &username, const QString &email, const QString &phone, const QString &old_password, const QString &new_password)
{
    User cur_user = SessionManager::getInstance().getCurrentUser();
    AuthResult result = authenticator.updateUser(cur_user.getId(), name, username, email, phone, old_password, new_password);
    int res;

    switch (result)
    {
    case AuthResult::SUCCESS:
        res = QMessageBox::information(main_window, "Success", "Profile updated successfully.");
        if (res == QMessageBox::Ok)
            main_window->showMainMenuPage();
        break;

    case AuthResult::USERNAME_TAKEN:
        QMessageBox::warning(main_window, "Error", "Username is already taken.");
        break;

    case AuthResult::EMAIL_TAKEN:
        QMessageBox::warning(main_window, "Error", "Email is already registered.");
        break;

    case AuthResult::PHONE_TAKEN:
        QMessageBox::warning(main_window, "Error", "Phone number is already registered.");
        break;

    case AuthResult::INVALID_EMAIL:
        QMessageBox::warning(main_window, "Error", "Invalid email format.");
        break;

    case AuthResult::INVALID_PHONE:
        QMessageBox::warning(main_window, "Error", "Invalid phone number format.");
        break;

    case AuthResult::WRONG_PASSWORD:
        QMessageBox::warning(main_window, "Error", "Current password is incorrect.");
        break;

    case AuthResult::WEAK_PASSWORD:
        QMessageBox::warning(main_window, "Error", "New password is too weak.");
        break;

    case AuthResult::EMPTY_FIELD:
        QMessageBox::warning(main_window, "Error", "Please fill out the required fields.");
        return;

    default:
        QMessageBox::warning(main_window, "Error", "An unknown error occurred.");
    }
}

void AppManager::handleShowHistory(const GameName game_name) {
    history_storage.reloadFromDisk();
    User cur_user = SessionManager::getInstance().getCurrentUser();
    QList<MatchRecord> user_history = history_storage.getHistoryForUser(cur_user.getId(), game_name);
    main_window->showHistoryPage(user_history, cur_user.getId(), game_name);
}

void AppManager::handleCreateRoom(const int port, const int board_size, const int time_limit, const GameName game_name)
{
    User cur_user = SessionManager::getInstance().getCurrentUser();
    main_window->showWatingHostPage(game_manager.createRoom(cur_user, port, game_name, board_size, time_limit));
}

void AppManager::handleJoinRoom(const QString &IP, const int port, const GameName game_name)
{
    User cur_user = SessionManager::getInstance().getCurrentUser();
    AuthResult result = authenticator.verifyIP(IP);

    switch (result)
    {
    case AuthResult::SUCCESS:
        game_manager.joinRoom(cur_user, IP, port, game_name);
        break;

    case AuthResult::INVALID_IP:
        QMessageBox::warning(main_window, "Error", "Invalid IP");
        break;

    default:
        QMessageBox::warning(main_window, "Error", "An unknown error occurred.");
    }
}

void AppManager::handleCancelHost()
{
    game_manager.cancelRoom();
}

void AppManager::handleLocalMove(int arg1, int arg2, int arg3)
{
    game_manager.handleLocalMove(arg1, arg2, arg3);
}

void AppManager::updateGameUI()
{
    Game *game_logic = game_manager.getCurrentGame();
    if (!game_logic)
        return;

    User cur_user = SessionManager::getInstance().getCurrentUser();
    bool is_my_turn = (game_logic->getCurrentPlayer().getId() == cur_user.getId());
    QString cur_user_username = cur_user.getUsername(), turn_text;
    if (is_my_turn)
        turn_text = ("Your Username: " + cur_user_username + "  |  Status: Your Turn");
    else
        turn_text = ("Your Username: " + cur_user_username + "  |  Status: Opponent's Turn");

    main_window->updateScoresAndTurn(game_logic->getFirstPlayerScore(), game_logic->getSecondPlayerScore(), turn_text, is_my_turn);

    main_window->renderActivePage(game_logic);
}

void AppManager::handleGameStarted()
{
    Game *game_logic = game_manager.getCurrentGame();
    if (!game_logic)
        return;

    GameName current_game_name = game_manager.getGameName();
    if (current_game_name == GameName::DotsAndBoxes) {
        int board_size = game_manager.getRoomBoardSize();
        main_window->showDotsAndBoxesPage(board_size);
    }
    else if (current_game_name == GameName::NineMensMorris)
        main_window->showNineMensMorrisPage();

    else if (current_game_name == GameName::Fanorona)
        main_window->showFanoronaPage();

    main_window->setPlayerNames(game_manager.getHostUsername(), game_manager.getGuestUsername());
    main_window->clearChat();
    updateGameUI();
}

void AppManager::handleMoveApplied(bool is_turn_kept)
{
    Q_UNUSED(is_turn_kept);
    updateGameUI();
}

void AppManager::handleOpponentMoveReceived()
{
    updateGameUI();
}

void AppManager::handleGameOver(GameStatus status, GameEndReason reason) {
    QString title_text;
    QString message_text;

    if (reason == GameEndReason::TIME_UP) {
        title_text = "Time's Up!";
        message_text = "The match is over. Time has expired!\n\n";
    }
    else if (reason == GameEndReason::RESIGNATION) {
        title_text = "Resignation";
        message_text = "The match ended due to a player resigning.\n\n";
    }
    else {
        title_text = "Match Result";
        message_text = "";
    }

    QString score_text = "";
    QString host_name = game_manager.getHostUsername();
    QString guest_name = game_manager.getGuestUsername();
    Game* game_logic = game_manager.getCurrentGame();
    if (game_logic) {
        int host_score = game_logic->getFirstPlayerScore();
        int guest_score = game_logic->getSecondPlayerScore();
        score_text = QString("\nScores:\n%1: %2\n%3: %4").arg(host_name).arg(host_score).arg(guest_name).arg(guest_score);
    }

    switch (status) {
    case GameStatus::HOST_WIN:
        message_text += "Host Wins the game!";
        break;
    case GameStatus::GUEST_WIN:
        message_text += "Guest Wins the game!";
        break;
    case GameStatus::DRAW:
        message_text += "The game ended in a Draw!";
        break;
    default:
        message_text += "Game Over!";
        break;
    }

    message_text += score_text;
    QMessageBox::information(main_window, title_text, message_text);
    main_window->showMainMenuPage();
}

void AppManager::handleChatMessageSend(const QString& text) {
    if (text.trimmed().isEmpty())
        return;

    game_manager.sendChatMessage(text);
    main_window->appendOwnChatMessage(text);
}

void AppManager::handleChatMessageReceived(const QString& sender_name, const QString& text) {
    main_window->receiveChatMessage(sender_name, text);
}

void AppManager::handleLocalPauseRequest() {
    QMessageBox::information(main_window, "Pause Request", "Pause request sent to the opponent. Waiting for response...");
    game_manager.requestPause();
}

void AppManager::handleOpponentPauseRequest() {
    QMessageBox::StandardButton reply = QMessageBox::question(main_window,
    "Pause Request", "Your opponent has requested to pause and save the game. Do you accept?", QMessageBox::Yes | QMessageBox::No);

    game_manager.respondToPause(reply == QMessageBox::Yes);
}

void AppManager::handlePauseResponse(bool accepted) {
    if (!accepted)
        QMessageBox::warning(main_window, "Pause Rejected", "Your opponent rejected the pause request.");
}

void AppManager::handleGamePausedSuccessfully() {
    QMessageBox::information(main_window, "Game Paused", "The match has been successfully paused and saved.");
    main_window->showMainMenuPage();
}