#include "forgot_password2.h"
#include "ui_forgot_password2.h"
#include <QMessageBox>

ForgotPassword2::ForgotPassword2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ForgotPassword2)
{
    ui->setupUi(this);

    ui->lineEdit_password->setPlaceholderText("Enter new Password");
    ui->lineEdit_newPassword->setPlaceholderText("Confirm Password");
}

ForgotPassword2::~ForgotPassword2()
{
    delete ui;
}

void ForgotPassword2::on_pushButton_reset_clicked()
{
    auto pass = ui->lineEdit_password->text();
    auto new_pass = ui->lineEdit_newPassword->text();

    if(pass.isEmpty() || new_pass.isEmpty()){
        QMessageBox::warning(this, "Restore Password", "Please fill out all fields.");
        return;
    }

    if(pass != new_pass){
        QMessageBox::warning(this, "Restore Password", "The Passwords do not match.");
        return;
    }

    int result = QMessageBox::information(this, "Success Message", "Password changed successfully.");
    if(result == QMessageBox::Ok){
        emit backToLogin();
    }
}


void ForgotPassword2::on_pushButton_cancel_clicked()
{
    emit backToLogin();
}

