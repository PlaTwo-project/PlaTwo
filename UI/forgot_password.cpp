#include "forgot_password.h"
#include "ui_forgot_password.h"
#include <QMessageBox>

ForgotPassword::ForgotPassword(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ForgotPassword)
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
    auto number = ui->lineEdit_phonenumber->text();
    if(number.isEmpty() || !true){
        QMessageBox::warning(this, "Restore Password", "Please Enter a valid phone number.");
        return;
    }

    emit goToPage2();
}


void ForgotPassword::on_pushButton_back_clicked()
{
    emit backToSignup();
}

