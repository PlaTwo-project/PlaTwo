#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

Login::Login(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    ui->lineEdit_username->setPlaceholderText("Enter your Username");
    ui->lineEdit_password->setPlaceholderText("Enter your Password");
}

Login::~Login()
{
    delete ui;
}

void Login::clearFields()
{
    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
}

void Login::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text().trimmed();
    QString password = ui->lineEdit_password->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login", "Please fill out all fields.");
        return;
    }

    emit loginRequested(username, password);
}

void Login::on_pushButton_signup_clicked()
{
    emit navigateToSignup(ui->lineEdit_username->text(), ui->lineEdit_password->text());
}

void Login::on_pushButton_forgotPassword_clicked()
{
    emit navigateToForgotPassword(ui->lineEdit_username->text());
}