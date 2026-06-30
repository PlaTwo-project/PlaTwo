#include "app_manager.h"
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
}

void AppManager::handleLogin(const QString& username, const QString& password)
{
    AuthResult result = authenticator.login(username, password);
    int res;

    switch (result)
    {
    case AuthResult::SUCCESS:
        res = QMessageBox::information(mainWindow, "Login", "Login successfully.");
        if(res == QMessageBox::Ok) {
            mainWindow->showMainMenu();
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
        QMessageBox::warning(mainWindow, "Error", "Unknown error.");
    }
}

void AppManager::handleSignup(const QString& name, const QString& username, const QString& email, const QString& phone, const QString& password)
{
    AuthResult result = authenticator.signup(name, username, email, phone, password);
    int res;

    switch (result)
    {
    case AuthResult::SUCCESS:
        res = QMessageBox::information(mainWindow, "Signup", "Account created successfully.");
        if(res == QMessageBox::Ok) {
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
        QMessageBox::warning(mainWindow, "Error", "Unknown error.");
    }
}

void AppManager::handleForgotPasswordStep2(const QString& username, const QString& phone)
{
    AuthResult result = authenticator.verifyUserPhone(username, phone);

    switch (result)
    {
    case AuthResult::SUCCESS:
        mainWindow->showForgotPassword2Page(username, phone);
        mainWindow->clearFPFields();
        break;

    case AuthResult::USER_NOT_FOUND:
        QMessageBox::warning(mainWindow, "Error", "User not found");
        break;

    case AuthResult::USER_PHONE_MISMATCH:
        QMessageBox::warning(mainWindow, "Error", "Username and phone do not match");
        break;

    default:
        QMessageBox::warning(mainWindow, "Error", "Unknown error");
    }
}

void AppManager::handleResetPassword(const QString& username, const QString& phone, const QString& newPassword)
{
    AuthResult result = authenticator.resetPassword(username, phone, newPassword);
    int res;

    switch (result)
    {
    case AuthResult::SUCCESS:
        res = QMessageBox::information(mainWindow, "Success Message", "Password changed successfully.");
        if(res == QMessageBox::Ok) {
            mainWindow->showLoginPage();
            mainWindow->clearFP2Fields();
        }
        break;

    case AuthResult::USER_NOT_FOUND:
        QMessageBox::warning(mainWindow, "Error", "User not found");
        break;

    case AuthResult::USER_PHONE_MISMATCH:
        QMessageBox::warning(mainWindow, "Error", "Username and phone do not match");
        break;

    case AuthResult::WEAK_PASSWORD:
        QMessageBox::warning(mainWindow, "Error", "Weak password");
        break;

    default:
        QMessageBox::warning(mainWindow, "Error", "Unknown error");
    }
}