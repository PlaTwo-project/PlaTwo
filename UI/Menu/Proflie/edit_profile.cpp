#include "edit_profile.h"
#include "ui_edit_profile.h"

#include <QMessageBox>

EditProfile::EditProfile(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditProfile)
{
    ui->setupUi(this);
}

EditProfile::~EditProfile()
{
    delete ui;
}

void EditProfile::on_pushButton_save_changes_clicked()
{
    QString name = ui->lineEdit_name->text().trimmed();
    QString username = ui->lineEdit_username->text().trimmed();
    QString email = ui->lineEdit_email->text().trimmed();
    QString phone = ui->lineEdit_phone->text().trimmed();
    QString oldPassword = ui->lineEdit_password->text();
    QString newPassword = ui->lineEdit->text();

    if (name.isEmpty() || username.isEmpty() || email.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Please fill in the required fields.");
        return;
    }

    emit editProfileRequested(name, username, email, phone, oldPassword, newPassword);
}

void EditProfile::on_pushButton_back_to_menu_clicked()
{
    emit navigateToMainMenu();
}