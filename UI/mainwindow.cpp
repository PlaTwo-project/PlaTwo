#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UI/Auth/Login/login.h"
#include "UI/Auth/SignUp/signup.h"
#include "UI/Auth/ForgotPassword/forgot_password.h"
#include "UI/Auth/ForgotPassword/forgot_password2.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //create pages
    Login* login = new Login(this);
    Signup* signup = new Signup(this);
    ForgotPassword* forgot_password = new ForgotPassword(this);
    ForgotPassword2* forgot_password2 = new ForgotPassword2(this);

    //add pages
    ui->stackedWidget->addWidget(login);
    ui->stackedWidget->addWidget(signup);
    ui->stackedWidget->addWidget(forgot_password);
    ui->stackedWidget->addWidget(forgot_password2);

    //signals
    connect(login, &Login::goToSignup, this, [this, signup](QString username, QString password) {
        signup->setInitialValues(username, password);
        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(login, &Login::goToForgotPassword, this, [this]() {
        ui->stackedWidget->setCurrentIndex(2);
    });

    connect(signup, &Signup::backToLogin, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(forgot_password, &ForgotPassword::goToPage2, this, [this]() {
        ui->stackedWidget->setCurrentIndex(3);
    });

    connect(forgot_password, &ForgotPassword::backToLogin, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(forgot_password2, &ForgotPassword2::backToLogin, this, [this, login](bool clearPass) {
        ui->stackedWidget->setCurrentIndex(0);
        if(clearPass) login->clearPass();
    });

    //set pages
    ui->stackedWidget->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}
