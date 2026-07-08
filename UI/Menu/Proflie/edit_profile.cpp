#include "edit_profile.h"
#include "ui_edit_profile.h"
#include <QMessageBox>

EditProfile::EditProfile(QWidget *parent)
    : QWidget(parent), ui(new Ui::EditProfile)
{
    ui->setupUi(this);

    ui->lineEdit_name->setPlaceholderText("Name");
    ui->lineEdit_username->setPlaceholderText("Username");
    ui->lineEdit_email->setPlaceholderText("Email");
    ui->lineEdit_phone->setPlaceholderText("Phone Number");
    ui->lineEdit_old_password->setPlaceholderText("Old Password");
    ui->lineEdit_new_password->setPlaceholderText("New Password");
}

EditProfile::~EditProfile()
{
    delete ui;
}

void EditProfile::setInitialValues(const QString &name, const QString &username, const QString &email, const QString &phone)
{
    ui->lineEdit_name->setText(name);
    ui->lineEdit_username->setText(username);
    ui->lineEdit_email->setText(email);
    ui->lineEdit_phone->setText(phone);
    ui->lineEdit_old_password->clear();
    ui->lineEdit_new_password->clear();
}

void EditProfile::on_pushButton_save_changes_clicked()
{
    QString name = ui->lineEdit_name->text().trimmed();
    QString username = ui->lineEdit_username->text().trimmed();
    QString email = ui->lineEdit_email->text().trimmed().trimmed();
    QString phone = ui->lineEdit_phone->text().trimmed().trimmed();
    QString old_password = ui->lineEdit_old_password->text().trimmed();
    QString new_password = ui->lineEdit_new_password->text().trimmed();

    if (name.isEmpty() || username.isEmpty() || email.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Please fill out the required fields.");
        return;
    }

    emit editProfileRequested(name, username, email, phone, old_password, new_password);
}

void EditProfile::on_pushButton_back_to_menu_clicked()
{
    emit navigateToMainMenu();
    ui->lineEdit_old_password->clear();
    ui->lineEdit_new_password->clear();
}