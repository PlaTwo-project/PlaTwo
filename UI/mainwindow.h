#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Login;
class Signup;
class ForgotPassword;
class ForgotPassword2;
class MainMenu;

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
    void showForgotPassword2Page(const QString& username, const QString& phone);
    void showMainMenu();

    void clearLoginFields();
    void clearSignupFields();
    void clearFPFields();
    void clearFP2Fields();

signals:
    void loginRequested(const QString& username, const QString& password);
    void signupRequested(const QString& name, const QString& username, const QString& email, const QString& phone, const QString& password);
    void forgotPasswordStep2Requested(const QString& username, const QString& phone);
    void resetPasswordRequested(const QString& username, const QString& phone, const QString& newPassword);

private:
    Ui::MainWindow *ui;

    Login* loginPage;
    Signup* signupPage;
    ForgotPassword* forgotPasswordPage;
    ForgotPassword2* forgotPassword2Page;
    MainMenu* mainMenuPage;
};

#endif // MAINWINDOW_H