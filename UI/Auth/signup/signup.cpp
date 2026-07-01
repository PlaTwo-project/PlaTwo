#include "signup.h"
#include "ui_signup.h"
#include <QMessageBox>

Signup::Signup(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::Signup)
{
    ui->setupUi(this);

    ui->lineEdit_name->setPlaceholderText("Enter your Name");
    ui->lineEdit_username->setPlaceholderText("Enter your Username");
    ui->lineEdit_email->setPlaceholderText("Enter your Email");
    ui->lineEdit_phonenumber->setPlaceholderText("Enter your Phone Number");
    ui->lineEdit_password->setPlaceholderText("Enter your Password");

}

Signup::~Signup()
{
    delete ui;
}

void Signup::setInitialValues(const QString& username, const QString& password)
{
    ui->lineEdit_username->setText(username);
    ui->lineEdit_password->setText(password);
}

void Signup::clearFields()
{
    ui->lineEdit_name->clear();
    ui->lineEdit_username->clear();
    ui->lineEdit_email->clear();
    ui->lineEdit_phonenumber->clear();
    ui->lineEdit_password->clear();
}

void Signup::on_pushButton_submit_clicked()
{
    QString name = ui->lineEdit_name->text().trimmed();
    QString username = ui->lineEdit_username->text().trimmed();
    QString phone = ui->lineEdit_phonenumber->text().trimmed();
    QString email = ui->lineEdit_email->text().trimmed();
    QString password = ui->lineEdit_password->text().trimmed();

    if (name.isEmpty() || username.isEmpty() || phone.isEmpty() || email.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Sign Up", "Please fill out all fields.");
        return;
    }

    emit signupRequested(name, username, email, phone, password);
}

void Signup::on_pushButton_login_clicked()
{
    emit navigateToLogin();
    clearFields();
}