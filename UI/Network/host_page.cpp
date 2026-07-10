#include "host_page.h"
#include "ui_host_page.h"

HostPage::HostPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HostPage)
{
    ui->setupUi(this);
}

HostPage::~HostPage()
{
    delete ui;
}

void HostPage::setPage(GameName game_name)
{
    if (game_name == GameName::DotsAndBoxes) {
        ui->label_game_name->setText("Boxes And Dots");
        ui->label_game_name_2->setText("Boxes And Dots");
    }
    else if (game_name == GameName::NineMensMorris) {
        ui->label_game_name->setText("Nine Men's Morris");
        ui->label_game_name_2->setText("Nine Men's Morris");
    }
    else if (game_name == GameName::Fanorona) {
        ui->label_game_name->setText("Fanorona");
        ui->label_game_name_2->setText("Fanorona");
    }

    ui->comboBox__board_size->clear();

    if (game_name == GameName::DotsAndBoxes) {
        ui->comboBox__board_size->addItem("6 x 6 Dots", 6);
        ui->comboBox__board_size->addItem("7 x 7 Dots", 7);
        ui->comboBox__board_size->addItem("8 x 8 Dots", 8);
    }
    else if (game_name == GameName::NineMensMorris) {
        ui->comboBox__board_size->addItem("Standard", 3);
    }
    else if (game_name == GameName::Fanorona) {
        ui->comboBox__board_size->addItem("Standard", 9);
    }

    ui->stackedWidget->setCurrentIndex(0);
}

void HostPage::on_pushButton_create_clicked()
{
    int port = ui->spinBox_port->value();
    int boardSize = ui->comboBox__board_size->currentData().toInt();

    int timeLimit = 0;
    if (ui->checkBox_time_limit->isChecked()) {
        timeLimit = ui->spinBox_time_limit->value();
    }

    emit createRoomRequested(port, boardSize, timeLimit);
}

void HostPage::on_pushButton_back_clicked()
{
    emit navigateToGameMenu();
}

void HostPage::on_pushButton_cancelHost_clicked()
{
    emit cancelHostRequested();
    ui->stackedWidget->setCurrentIndex(0);
}

void HostPage::switchToWaitingStatus(const QString& Ip)
{
    ui->label_server_IP->setText("IP Address: " + Ip);
    ui->stackedWidget->setCurrentIndex(1);
}