#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UI/Auth/Login/login.h"
#include "UI/Auth/SignUp/signup.h"
#include "UI/Auth/ForgotPassword/forgot_password.h"
#include "UI/Auth/ForgotPassword/forgot_password2.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loginPage = new Login(this);
    signupPage = new Signup(this);
    forgotPasswordPage = new ForgotPassword(this);
    forgotPassword2Page = new ForgotPassword2(this);

    ui->stackedWidget->addWidget(loginPage);
    ui->stackedWidget->addWidget(signupPage);
    ui->stackedWidget->addWidget(forgotPasswordPage);
    ui->stackedWidget->addWidget(forgotPassword2Page);

    connect(loginPage, &Login::navigateToSignup, this, [this]() {
        showSignupPage();
    });

    connect(loginPage, &Login::navigateToForgotPassword, this, [this]() {
        showForgotPasswordPage();
    });

    connect(signupPage, &Signup::navigateToLogin, this, [this]() {
        showLoginPage();
    });

    connect(forgotPasswordPage, &ForgotPassword::navigateToLogin, this, [this]() {
        showLoginPage();
    });

    connect(forgotPassword2Page, &ForgotPassword2::navigateToLogin, this, [this]() {
        showLoginPage();
    });

    connect(loginPage, &Login::loginRequested, this, &MainWindow::loginRequested);
    connect(signupPage, &Signup::signupRequested, this, &MainWindow::signupRequested);
    connect(forgotPasswordPage, &ForgotPassword::forgotPasswordStep2Requested, this, &MainWindow::forgotPasswordStep2Requested);
    connect(forgotPassword2Page, &ForgotPassword2::resetPasswordRequested, this, &MainWindow::resetPasswordRequested);

    showLoginPage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showLoginPage()
{
    ui->stackedWidget->setCurrentWidget(loginPage);
}

void MainWindow::showSignupPage()
{
    ui->stackedWidget->setCurrentWidget(signupPage);
}

void MainWindow::showForgotPasswordPage()
{
    ui->stackedWidget->setCurrentWidget(forgotPasswordPage);
}

void MainWindow::showForgotPassword2Page(const QString& username, const QString &phone)
{
    forgotPassword2Page->setUserData(username, phone);
    ui->stackedWidget->setCurrentWidget(forgotPassword2Page);
}