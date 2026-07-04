#include "guest_page.h"
#include "ui_guest_page.h"
#include <QMessageBox>

GuestPage::GuestPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GuestPage)
{
    ui->setupUi(this);

    ui->lineEdit_ip->setPlaceholderText("e.g. 127.0.0.1");
    ui->lineEdit_port->setPlaceholderText("Port must be free");
}

void GuestPage::setTitle(GameName game_name)
{
    if (game_name == GameName::BoxesAndDots)
        ui->label_game->setText("Boxes And Dots");

    else if (game_name == GameName::NineMensMorris)
        ui->label_game->setText("Nine Men's Morris");

    else if (game_name == GameName::Fanorona)
        ui->label_game->setText("Fanorona");
}

void GuestPage::clearFields()
{
    ui->lineEdit_ip->clear();
    ui->lineEdit_port->clear();
}

GuestPage::~GuestPage()
{
    delete ui;
}

void GuestPage::on_pushButton_join_clicked()
{
    QString ip = ui->lineEdit_ip->text().trimmed();
    QString port = ui->lineEdit_port->text().trimmed();

    if (ip.isEmpty() || port.isEmpty()) {
        QMessageBox::warning(this, "Guest Page", "Please fill out all fields.");
        return;
    }

    emit joinRequested(ip, port);
}

void GuestPage::on_pushButton_back_clicked()
{
    clearFields();
    emit navigateToGameMenu();
}