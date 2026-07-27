#include "forgot_password.h"
#include "ui_forgot_password.h"
#include "UI/MainWindow/mainwindow.h"
#include "UI/Exception/forget_password_exception.h"
#include "UI/page_transition.h"
#include "Logic/Constants/const_numbers.h"

ForgotPassword::ForgotPassword(QWidget *parent) : QWidget(parent), ui(new Ui::ForgotPassword) {
    ui->setupUi(this);

    ui->lineEdit_username_2->setPlaceholderText("Username");
    ui->lineEdit_phonenumber_2->setPlaceholderText("Phone Number");
    ui->lineEdit_password->setPlaceholderText("Enter new Password");
    ui->lineEdit_new_password->setPlaceholderText("Confirm Password");
}

ForgotPassword::~ForgotPassword() {
    delete ui;
}

void ForgotPassword::setInitialValues(const QString& username) {
    ui->lineEdit_username_2->setText(username);
}

void ForgotPassword::setUserData(const QString& username, const QString& phone) {
    phone_number = phone;
    user_name = username;
}

void ForgotPassword::switchToVerifyPage() {
    pageTransition(ui->stackedWidget, ui->stackedWidget->widget(VERIFY_PAGE), this);
}

void ForgotPassword::switchToResetPage() {
    pageTransition(ui->stackedWidget, ui->stackedWidget->widget(RESET_PAGE), this);
}

void ForgotPassword::clearFields() {
    ui->lineEdit_username_2->clear();
    ui->lineEdit_phonenumber_2->clear();
    ui->lineEdit_password->clear();
    ui->lineEdit_new_password->clear();
}

void ForgotPassword::on_pushButton_next_2_clicked() {
    try {
        QString phone = ui->lineEdit_phonenumber_2->text().trimmed();
        QString username = ui->lineEdit_username_2->text().trimmed();
        if (phone.isEmpty() || username.isEmpty())
            throw ForgotPasswordException("Please fill out all fields.");

        emit forgotPasswordStep2Requested(username, phone);
    }
    catch (const BaseException& e) {
        e.showDialog(this);
    }
}

void ForgotPassword::on_pushButton_back_2_clicked() {
    clearFields();
    emit navigateToLogin();
}

void ForgotPassword::on_pushButton_reset_clicked() {
    try {
        QString pass = ui->lineEdit_password->text().trimmed();
        QString confirm = ui->lineEdit_new_password->text().trimmed();
        if (pass.isEmpty() || confirm.isEmpty())
            throw ForgotPasswordException("Please fill out all fields.");

        if (pass != confirm)
            throw ForgotPasswordException("Passwords do not match.");

        emit resetPasswordRequested(user_name, phone_number, pass);
    }
    catch (const BaseException& e) {
        e.showDialog(this);
    }
}

void ForgotPassword::on_pushButton_cancel_clicked() {
    emit navigateToLogin();

    ui->lineEdit_password->clear();
    ui->lineEdit_new_password->clear();
}