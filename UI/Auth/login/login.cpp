#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    ui->lineEdit_username->setPlaceholderText("Enter your Username");
    ui->lineEdit_password->setPlaceholderText("Enter your Password");
}

void Login::clearPass(){
    ui->lineEdit_password->clear();
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_login_clicked()
{
    auto username = ui->lineEdit_username->text();
    auto password = ui->lineEdit_password->text();

    if(username.isEmpty() || password.isEmpty()){
        QMessageBox::warning(this, "Login", "Please fill out all fields.");
        return;
    }
}

void Login::on_pushButton_signup_clicked()
{
    emit goToSignup(ui->lineEdit_username->text(), ui->lineEdit_password->text());
}

void Login::on_pushButton_forgotPassword_clicked()
{
    emit goToForgotPassword();
}
