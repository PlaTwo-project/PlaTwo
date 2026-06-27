#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UI/login.h"
#include "UI/signup.h"
#include "UI/forgot_password.h"
#include "UI/forgot_password2.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Login* login = new Login(this);
    Signup* signup = new Signup(this);
    ForgotPassword* forgot_password = new ForgotPassword(this);
    ForgotPassword2* forgot_password2 = new ForgotPassword2(this);

    ui->stackedWidget->addWidget(login);
    ui->stackedWidget->addWidget(signup);
    ui->stackedWidget->addWidget(forgot_password);
    ui->stackedWidget->addWidget(forgot_password2);

    connect(login, &Login::goToSignup, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(login, &Login::goToForgotPassword, [this]() {
        ui->stackedWidget->setCurrentIndex(2);
    });

    connect(signup, &Signup::backToLogin, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(forgot_password, &ForgotPassword::goToPage2, [this]() {
        ui->stackedWidget->setCurrentIndex(3);
    });

    connect(forgot_password, &ForgotPassword::backToLogin, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(forgot_password2, &ForgotPassword2::backToLogin, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    ui->stackedWidget->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}
