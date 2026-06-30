#include "main_menu.h"
#include "ui_main_menu.h"
#include <QMessageBox>

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_pushButton_dots_clicked()
{

}

void MainMenu::on_pushButton_morris_clicked()
{

}

void MainMenu::on_pushButton_fanorona_clicked()
{

}

void MainMenu::on_pushButton_history_clicked()
{

}

void MainMenu::on_pushButton_editProfile_clicked()
{
    emit navigateToEditProfile();
}

void MainMenu::on_pushButton_logout_clicked()
{
    emit navigateToLogin();
}

void MainMenu::on_pushButton_exit_clicked()
{
    auto reply = QMessageBox::question(this, "Exit", "Are you sure you want to exit?", QMessageBox::No | QMessageBox::Cancel | QMessageBox::NoAll | QMessageBox::Discard);
    if (reply == QMessageBox::Yes)
        QApplication::quit();
}