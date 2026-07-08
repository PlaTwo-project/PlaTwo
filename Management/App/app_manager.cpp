#include "Management/App/app_manager.h"
#include "session_manager.h"
#include <QMessageBox>

AppManager::AppManager(QObject* parent)
    : QObject{parent}, userStorage("users.json"), authenticator(userStorage)
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
            main_window->clearFP2Fields();
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
        QMessageBox::warning(main_window, "Warning", "Please fill out the required fields.");
        return;

    default:
        QMessageBox::warning(main_window, "Error", "An unknown error occurred.");
    }
}