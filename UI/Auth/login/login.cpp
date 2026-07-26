#include "login.h"
#include "ui_login.h"
#include "UI/Exception/login_exception.h"

Login::Login(QWidget *parent) : QWidget(parent), ui(new Ui::Login) {
    ui->setupUi(this);

    ui->lineEdit_username->setPlaceholderText("Enter your Username");
    ui->lineEdit_password->setPlaceholderText("Enter your Password");
}

Login::~Login() {
    delete ui;
}

void Login::clearFields() {
    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
}

void Login::on_pushButton_login_clicked() {
    try {
        QString username = ui->lineEdit_username->text().trimmed();
        QString password = ui->lineEdit_password->text().trimmed();
        if (username.isEmpty() || password.isEmpty())
            throw LoginException("Please fill out all fields.");

        emit loginRequested(username, password);
    }
    catch (const BaseException& e) {
        e.showDialog(this);
    }
}

void Login::on_pushButton_signup_clicked() {
    emit navigateToSignup(ui->lineEdit_username->text(), ui->lineEdit_password->text());
}

void Login::on_pushButton_forgotPassword_clicked() {
    emit navigateToForgotPassword(ui->lineEdit_username->text());
}

void Login::showEvent(QShowEvent *event) { // set focus on username-field
    QWidget::showEvent(event);
    ui->lineEdit_username->setFocus();
}