 #include "game_menu.h"
#include "ui_game_menu.h"
#include "QMessageBox"

GameMenu::GameMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameMenu)
{
    ui->setupUi(this);
}

void GameMenu::setTitle(GameName game_name)
{
    cur_game = game_name;
    if (game_name == GameName::DotsAndBoxes)
        ui->label_menu_title->setText("Boxes And Dots");

    else if (game_name == GameName::NineMensMorris)
        ui->label_menu_title->setText("Nine Men's Morris");

    else if (game_name == GameName::Fanorona)
        ui->label_menu_title->setText("Fanorona");
}

GameMenu::~GameMenu()
{
    delete ui;
}

void GameMenu::on_pushButton_start_game_clicked()
{
    auto reply = QMessageBox::question(this, "Start Game", "Would you like to be a host?\nif not, you'll be a guest", QMessageBox::No | QMessageBox::Yes);

    if (reply == QMessageBox::Yes)
        emit navigateToHostPage(cur_game);
    else if (reply == QMessageBox::No)
        emit navigateToGuestPage(cur_game);
}

void GameMenu::on_pushButton_history_clicked()
{
    emit navigateToHistory(cur_game);
}

void GameMenu::on_pushButton_back_clicked()
{
    emit navigateToMainMenu();
}

