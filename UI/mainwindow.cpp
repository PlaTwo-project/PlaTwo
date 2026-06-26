#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UI/signup.h"
#include "UI/forgot_password.h"
#include "UI/forgot_password2.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //create pages
    Signup* signup = new Signup(this);
    ForgotPassword* forgot_password = new ForgotPassword(this);
    ForgotPassword2* forgot_password2 = new ForgotPassword2(this);

    //add pages
    ui->stackedWidget->addWidget(signup);
    ui->stackedWidget->addWidget(forgot_password);
    ui->stackedWidget->addWidget(forgot_password2);

    //signals
    connect(forgot_password, &ForgotPassword::goToPage2, [this]() {
        ui->stackedWidget->setCurrentIndex(2);
    });

    connect(forgot_password, &ForgotPassword::backToSignup, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(forgot_password2, &ForgotPassword2::backToLogin, [this]() {
        ui->stackedWidget->setCurrentIndex(0); //inja indexe login page ro bezar
    });

    connect(signup, &Signup::backToLogin, [this]() {
        ui->stackedWidget->setCurrentIndex(0); //inja ham hamintor
    });

    //set pages
    ui->stackedWidget->setCurrentIndex(0); //inja ham hamintor
}

MainWindow::~MainWindow()
{
    delete ui;
}