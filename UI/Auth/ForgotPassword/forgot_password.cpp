#include "forgot_password.h"
#include "ui_forgot_password.h"
#include <QMessageBox>
#include "UI/mainwindow.h"

ForgotPassword::ForgotPassword(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::ForgotPassword)
{
    ui->setupUi(this);

    ui->lineEdit_username->setPlaceholderText("Username");
    ui->lineEdit_phonenumber->setPlaceholderText("Phone Number");
}

ForgotPassword::~ForgotPassword()
{
    delete ui;
}

void ForgotPassword::setInitialValues(const QString& username)
{
    ui->lineEdit_username->setText(username);
}

void ForgotPassword::clearFields()
{
    ui->lineEdit_username->clear();
    ui->lineEdit_phonenumber->clear();
}

void ForgotPassword::on_pushButton_next_clicked()
{
    QString phone = ui->lineEdit_phonenumber->text().trimmed();
    QString username = ui->lineEdit_username->text().trimmed();

    if (phone.isEmpty() || username.isEmpty()) {
        QMessageBox::warning(this, "Restore Password", "Please fill out all fields.");
        return;
    }

    emit forgotPasswordStep2Requested(username, phone);
}

void ForgotPassword::on_pushButton_back_clicked()
{
    clearFields();
    emit navigateToLogin();
}