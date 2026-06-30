#include "forgot_password2.h"
#include "ui_forgot_password2.h"

#include <QMessageBox>

ForgotPassword2::ForgotPassword2(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::ForgotPassword2)
{
    ui->setupUi(this);

    ui->lineEdit_password->setPlaceholderText("Enter new Password");
    ui->lineEdit_newPassword->setPlaceholderText("Confirm Password");
}

ForgotPassword2::~ForgotPassword2()
{
    delete ui;
}

void ForgotPassword2::setUserData(const QString& username, const QString& phone)
{
    phone_number = phone;
    user_name = username;
}

void ForgotPassword2::on_pushButton_reset_clicked()
{
    QString pass = ui->lineEdit_password->text();
    QString confirm = ui->lineEdit_newPassword->text();

    if (pass.isEmpty() || confirm.isEmpty()) {
        QMessageBox::warning(this, "Restore Password", "Please fill out all fields.");
        return;
    }

    if (pass != confirm) {
        QMessageBox::warning(this, "Restore Password", "Passwords do not match.");
        return;
    }

    ui->lineEdit_password->clear();
    ui->lineEdit_newPassword->clear();

    emit resetPasswordRequested(user_name, phone_number, pass);
}

void ForgotPassword2::on_pushButton_cancel_clicked()
{
    ui->lineEdit_password->clear();
    ui->lineEdit_newPassword->clear();

    emit navigateToLogin();
}