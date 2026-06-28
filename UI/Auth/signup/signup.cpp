#include "signup.h"
#include "ui_signup.h"
#include <QMessageBox>

Signup::Signup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Signup)
{
    ui->setupUi(this);

    ui->lineEdit_password->setPlaceholderText("Enter your Password");
    ui->lineEdit_username->setPlaceholderText("Enter your Username");
    ui->lineEdit_name->setPlaceholderText("Enter your Name");
    ui->lineEdit_phonenumber->setPlaceholderText("Enter your Phone Number");
    ui->lineEdit_email->setPlaceholderText("Enter your Email");
}

void Signup::setInitialValues(const QString &username, const QString &password)
{
    ui->lineEdit_username->setText(username);
    ui->lineEdit_password->setText(password);
}


Signup::~Signup()
{
    delete ui;
}

void Signup::on_pushButton_submit_clicked()
{
    auto name = ui->lineEdit_name->text();
    auto username = ui->lineEdit_username->text();
    auto phone_number = ui->lineEdit_phonenumber->text();
    auto email = ui->lineEdit_email->text();
    auto password = ui->lineEdit_password->text();

    if(name.isEmpty() || username.isEmpty() || phone_number.isEmpty() || email.isEmpty() || password.isEmpty()){
        QMessageBox::warning(this, "Sign up", "Please fill out all fields.");
        return;
    }
}


void Signup::on_pushButton_login_clicked()
{
    emit backToLogin();
    ui->lineEdit_email->clear();
    ui->lineEdit_phonenumber->clear();
    ui->lineEdit_name->clear();
}