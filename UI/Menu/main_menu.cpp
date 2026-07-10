#include "main_menu.h"
#include "ui_main_menu.h"
#include <QMessageBox>
#include "UI/Menu/game_menu.h"

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
    emit navigateToGame1(GameName::DotsAndBoxes);
}

void MainMenu::on_pushButton_morris_clicked()
{
    emit navigateToGame2(GameName::NineMensMorris);
}

void MainMenu::on_pushButton_fanorona_clicked()
{
    emit navigateToGame3(GameName::Fanorona);
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
    auto reply = QMessageBox::question(this, "Exit", "Are you sure you want to exit?", QMessageBox::No | QMessageBox::Yes);
    if (reply == QMessageBox::Yes)
        QApplication::quit();
}