#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Login;
class Signup;
class ForgotPassword;
class ForgotPassword2;
class MainMenu;
class EditProfile;

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
    void editProfileRequested(const QString& name, const QString& username, const QString& email, const QString& phone, const QString& new_password, const QString& old_password);

private:
    Ui::MainWindow *ui;

    Login* login_page;
    Signup* signup_page;
    ForgotPassword* forgot_password_page;
    ForgotPassword2* forgot_password_page2;
    MainMenu* main_menu_page;
    EditProfile* edit_profile_page;
};

#endif // MAINWINDOW_H