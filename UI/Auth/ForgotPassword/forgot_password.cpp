#include "forgot_password.h"
#include "ui_forgot_password.h"

#include <QMessageBox>

ForgotPassword::ForgotPassword(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::ForgotPassword)
{
    ui->setupUi(this);

    ui->lineEdit_phonenumber->setPlaceholderText("Phone Number");
}

ForgotPassword::~ForgotPassword()
{
    delete ui;
}

void ForgotPassword::on_pushButton_next_clicked()
{
    QString phone = ui->lineEdit_phonenumber->text();
    QString username = ui->lineEdit_username->text();

    if (phone.isEmpty() || username.isEmpty()) {
        QMessageBox::warning(this, "Restore Password", "Please enter your phone number.");
        return;
    }

    ui->lineEdit_phonenumber->clear();

    emit forgotPasswordStep2Requested(username, phone);
}

void ForgotPassword::on_pushButton_back_clicked()
{
    ui->lineEdit_phonenumber->clear();

    emit navigateToLogin();
}