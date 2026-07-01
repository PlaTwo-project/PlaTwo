#include "Management/App/app_manager.h"
#include "session_manager.h"
#include <QMessageBox>

AppManager::AppManager(QObject *parent)
    : QObject{parent}
{
    mainWindow = new MainWindow();
    setupConnections();
}

void AppManager::run()
{
    mainWindow->show();
}

void AppManager::setupConnections()
{
    connect(mainWindow, &MainWindow::loginRequested, this, &AppManager::handleLogin);

    connect(mainWindow, &MainWindow::signupRequested, this, &AppManager::handleSignup);

    connect(mainWindow, &MainWindow::forgotPasswordStep2Requested, this, &AppManager::handleForgotPasswordStep2);

    connect(mainWindow, &MainWindow::resetPasswordRequested, this, &AppManager::handleResetPassword);

    connect(mainWindow, &MainWindow::editProfileRequested, this, &AppManager::handleEditProfile);
}

void AppManager::handleLogin(const QString& username, const QString& password)
{
    User loggedInUser;
    AuthResult result = authenticator.login(username, password, loggedInUser);
    int res;

    switch (result) {
    case AuthResult::SUCCESS:
        SessionManager::getInstance().login(loggedInUser);
        res = QMessageBox::information(mainWindow, "Login", "Login successfully.");
        if (res == QMessageBox::Ok) {
            User user = SessionManager::getInstance().getCurrentUser();
            mainWindow->loadUserDataInProfile(user.getName(), user.getUsername(), user.getEmail(), user.getPhoneNumber());
            mainWindow->showMainMenuPage();
            mainWindow->clearLoginFields();
        }
        break;

    case AuthResult::USER_NOT_FOUND:
        QMessageBox::warning(mainWindow, "Error", "User not found.");
        break;

    case AuthResult::WRONG_PASSWORD:
        QMessageBox::warning(mainWindow, "Error", "Wrong password.");
        break;

    default:
        QMessageBox::warning(mainWindow, "Error", "An unknown error occurred.");
    }
}

void AppManager::handleSignup(const QString& name, const QString& username, const QString& email, const QString& phone, const QString& password)
{
    AuthResult result = authenticator.signup(name, username, email, phone, password);
    int res;

    switch (result) {
    case AuthResult::SUCCESS:
        res = QMessageBox::information(mainWindow, "Signup", "Account created successfully.");
        if (res == QMessageBox::Ok) {
            mainWindow->showLoginPage();
            mainWindow->clearSignupFields();
        }
        break;

    case AuthResult::USERNAME_TAKEN:
        QMessageBox::warning(mainWindow, "Error", "Username already taken.");
        break;

    case AuthResult::EMAIL_TAKEN:
        QMessageBox::warning(mainWindow, "Error", "Email already used.");
        break;

    case AuthResult::PHONE_TAKEN:
        QMessageBox::warning(mainWindow, "Error", "Phone number already used.");
        break;

    case AuthResult::INVALID_EMAIL:
        QMessageBox::warning(mainWindow, "Error", "Invalid email.");
        break;

    case AuthResult::INVALID_PHONE:
        QMessageBox::warning(mainWindow, "Error", "Invalid phone number.");
        break;

    case AuthResult::WEAK_PASSWORD:
        QMessageBox::warning(mainWindow, "Error", "Weak password.");
        break;

    default:
        QMessageBox::warning(mainWindow, "Error", "An unknown error occurred.");
    }
}

void AppManager::handleForgotPasswordStep2(const QString& username, const QString& phone)
{
    AuthResult result = authenticator.verifyUserPhone(username, phone);

    switch (result) {
    case AuthResult::SUCCESS:
        mainWindow->showForgotPasswordPage2(username, phone);
        mainWindow->clearFPFields();
        break;

    case AuthResult::USER_NOT_FOUND:
        QMessageBox::warning(mainWindow, "Error", "User not found.");
        break;

    case AuthResult::USER_PHONE_MISMATCH:
        QMessageBox::warning(mainWindow, "Error", "Username and phone do not match.");
        break;

    default:
        QMessageBox::warning(mainWindow, "Error", "An unknown error occurred.");
    }
}

void AppManager::handleResetPassword(const QString& username, const QString& phone, const QString& newPassword)
{
    AuthResult result = authenticator.resetPassword(username, phone, newPassword);
    int res;

    switch (result) {
    case AuthResult::SUCCESS:
        res = QMessageBox::information(mainWindow, "Success Message", "Password changed successfully.");
        if (res == QMessageBox::Ok) {
            mainWindow->showLoginPage();
            mainWindow->clearFP2Fields();
        }
        break;

    case AuthResult::USER_NOT_FOUND:
        QMessageBox::warning(mainWindow, "Error", "User not found.");
        break;

    case AuthResult::USER_PHONE_MISMATCH:
        QMessageBox::warning(mainWindow, "Error", "Username and phone do not match.");
        break;

    case AuthResult::WEAK_PASSWORD:
        QMessageBox::warning(mainWindow, "Error", "Weak password.");
        break;

    default:
        QMessageBox::warning(mainWindow, "Error", "An unknown error occurred.");
    }
}

void AppManager::handleEditProfile(const QString& name, const QString& username, const QString& email, const QString& phone, const QString& oldPassword, const QString& newPassword)
{
    User currentUser = SessionManager::getInstance().getCurrentUser();
    AuthResult result = authenticator.updateUser(currentUser.getId(), name, username, email, phone, oldPassword, newPassword);
    int res;

    switch (result) {
    case AuthResult::SUCCESS:
        res = QMessageBox::information(mainWindow, "Success", "Profile updated successfully.");
        if (res == QMessageBox::Ok)
            mainWindow->showMainMenuPage();
        break;

    case AuthResult::USERNAME_TAKEN:
        QMessageBox::warning(mainWindow, "Error", "Username is already taken.");
        break;

    case AuthResult::EMAIL_TAKEN:
        QMessageBox::warning(mainWindow, "Error", "Email is already registered.");
        break;

    case AuthResult::PHONE_TAKEN:
        QMessageBox::warning(mainWindow, "Error", "Phone number is already registered.");
        break;

    case AuthResult::INVALID_EMAIL:
        QMessageBox::warning(mainWindow, "Error", "Invalid email format.");
        break;

    case AuthResult::INVALID_PHONE:
        QMessageBox::warning(mainWindow, "Error", "Invalid phone number format.");
        break;

    case AuthResult::WRONG_PASSWORD:
        QMessageBox::warning(mainWindow, "Error", "Current password is incorrect.");
        break;

    case AuthResult::WEAK_PASSWORD:
        QMessageBox::warning(mainWindow, "Error", "New password is too weak.");
        break;

    case AuthResult::EMPTY_FIELD:
        QMessageBox::warning(mainWindow, "Warning", "Please fill out the required fields.");
        return;

    default:
        QMessageBox::warning(mainWindow, "Error", "An unknown error occurred.");
    }
}