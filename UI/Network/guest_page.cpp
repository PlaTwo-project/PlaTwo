#include "guest_page.h"
#include "ui_guest_page.h"
#include "UI/Games/DotsAndBoxesPage/ColorSelectionDialog/color_selection_dialog.h"
#include <QMessageBox>
#include <QGraphicsBlurEffect>

GuestPage::GuestPage(QWidget *parent) : QWidget(parent) , ui(new Ui::GuestPage) {
    ui->setupUi(this);
    ui->lineEdit_ip->setPlaceholderText("e.g. 127.0.0.1");
    ui->lineEdit_port->setPlaceholderText("Port must be free");
}

void GuestPage::setTitle(GameName game_name) {
    current_game_name = game_name;
    if (game_name == GameName::DotsAndBoxes)
        ui->label_game->setText("Boxes And Dots");

    else if (game_name == GameName::NineMensMorris)
        ui->label_game->setText("Nine Men's Morris");

    else if (game_name == GameName::Fanorona)
        ui->label_game->setText("Fanorona");
}

void GuestPage::clearFields() {
    ui->lineEdit_ip->clear();
    ui->lineEdit_port->clear();
}

GuestPage::~GuestPage() {
    delete ui;
}

void GuestPage::on_pushButton_join_clicked() {
    QString ip = ui->lineEdit_ip->text().trimmed();
    QString text_port = ui->lineEdit_port->text().trimmed();
    if (ip.isEmpty() || text_port.isEmpty()) {
        QMessageBox::warning(this, "Guest Page", "Please fill out all fields.");
        return;
    }

    int port = text_port.toInt();
    int colorIndex = -1;
    if (current_game_name == GameName::DotsAndBoxes) {
        QGraphicsBlurEffect* blur_effect = new QGraphicsBlurEffect(this);
        blur_effect->setBlurRadius(15);
        this->setGraphicsEffect(blur_effect);
        ColorSelectionDialog color_dialog(this);
        int result = color_dialog.exec();
        this->setGraphicsEffect(nullptr);

        if (result != QDialog::Accepted)
            return;

        colorIndex = color_dialog.selectedColorIndex();
    }

    emit joinRequested(ip, port, colorIndex);
}

void GuestPage::on_pushButton_back_clicked() {
    clearFields();
    emit navigateToGameMenu();
}