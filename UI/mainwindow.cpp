#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UI/Auth/Login/login.h"
#include "UI/Auth/SignUp/signup.h"
#include "UI/Auth/ForgotPassword/forgot_password.h"
#include "UI/Auth/ForgotPassword/forgot_password2.h"
#include "UI/Menu/main_menu.h"
#include "UI/Menu/Proflie/edit_profile.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //create pages
    login_page = new Login(this);
    signup_page = new Signup(this);
    forgot_password_page = new ForgotPassword(this);
    forgot_password_page2 = new ForgotPassword2(this);
    main_menu_page = new MainMenu(this);
    edit_profile_page = new EditProfile(this);

    // add pages
    ui->stackedWidget->addWidget(login_page);
    ui->stackedWidget->addWidget(signup_page);
    ui->stackedWidget->addWidget(forgot_password_page);
    ui->stackedWidget->addWidget(forgot_password_page2);
    ui->stackedWidget->addWidget(main_menu_page);
    ui->stackedWidget->addWidget(edit_profile_page);

    // received signals
    connect(login_page, &Login::navigateToSignup, this, [this](QString username, QString password) {
        signup_page->setInitialValues(username, password);
        showSignupPage();
    });

    connect(login_page, &Login::navigateToForgotPassword, this, [this](QString username) {
        forgot_password_page->setInitialValues(username);
        showForgotPasswordPage();
    });

    connect(signup_page, &Signup::navigateToLogin, this, [this]() {
        showLoginPage();
    });

    connect(forgot_password_page, &ForgotPassword::navigateToLogin, this, [this]() {
        showLoginPage();
    });

    connect(forgot_password_page2, &ForgotPassword2::navigateToLogin, this, [this]() {
        showLoginPage();
    });

    // page transfer signals
    connect(login_page, &Login::loginRequested, this, &MainWindow::loginRequested);

    connect(signup_page, &Signup::signupRequested, this, &MainWindow::signupRequested);

    connect(forgot_password_page, &ForgotPassword::forgotPasswordStep2Requested, this, &MainWindow::forgotPasswordStep2Requested);

    connect(forgot_password_page2, &ForgotPassword2::resetPasswordRequested, this, &MainWindow::resetPasswordRequested);

    connect(main_menu_page, &MainMenu::navigateToLogin, this, &MainWindow::showLoginPage);

    connect(main_menu_page, &MainMenu::navigateToEditProfile, this, &MainWindow::showEditProfilePage);

    connect(edit_profile_page, &EditProfile::navigateToMainMenu, this, &MainWindow::showMainMenuPage);

    connect(edit_profile_page, &EditProfile::editProfileRequested, this, &MainWindow::editProfileRequested);


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
    ui->stackedWidget->setCurrentWidget(forgot_password_page);
}

void MainWindow::showForgotPasswordPage2(const QString& username, const QString &phone)
{
    forgot_password_page2->setUserData(username, phone);
    ui->stackedWidget->setCurrentWidget(forgot_password_page2);
}

void MainWindow::showMainMenuPage()
{
    ui->stackedWidget->setCurrentWidget(main_menu_page);
}

void MainWindow::showEditProfilePage()
{
    ui->stackedWidget->setCurrentWidget(edit_profile_page);
}

void MainWindow::loadUserDataInProfile(const QString& name, const QString& username, const QString& email, const QString& phone)
{
    edit_profile_page->setInitialValues(name, username, email, phone);
}

void MainWindow::clearLoginFields()
{
    login_page->clearFields();
}

void MainWindow::clearSignupFields()
{
    signup_page->clearFields();
}

void MainWindow::clearFPFields()
{
    forgot_password_page->clearFields();
}

void MainWindow::clearFP2Fields()
{
    forgot_password_page2->clearFields();
}