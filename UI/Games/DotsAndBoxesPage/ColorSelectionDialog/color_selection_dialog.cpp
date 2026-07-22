#include "color_selection_dialog.h"
#include "ui_color_selection_dialog.h"
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesColors/dots_and_boxes_colors.h"

#include <QButtonGroup>

ColorSelectionDialog::ColorSelectionDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ColorSelectionDialog), selected_index(-1) {
    ui->setupUi(this);
    color_buttons = {
        ui->pushButton_red,
        ui->pushButton_blue,
        ui->pushButton_green,
        ui->pushButton_orange,
        ui->pushButton_purple,
        ui->pushButton_yellow,
        ui->pushButton_teal,
        ui->pushButton_pink,
        ui->pushButton_brown
    };

    const QVector<QColor>& colors = DotsAndBoxesColors::palette();
    for (int i = 0; i < color_buttons.size(); ++i) {
        QPushButton* button = color_buttons[i];
        QString hex = colors[i].name();

        button->setProperty("baseStyle", QString("QPushButton {"
                                                 "background-color: %1;"
                                                 "border-radius: 8px;"
                                                 "border: 2px solid #444;"
                                                 "}"

                                                 "QPushButton:hover {"
                                                 "border: 2px solid #000;"
                                                 "}"

                                                 ).arg(hex));

        button->setStyleSheet(button->property("baseStyle").toString());
        connect(button, &QPushButton::clicked, this, [this, i]() {
            selectColor(i);
        });
    }
}

void ColorSelectionDialog::selectColor(int index) {
    selected_index = index;
    for (int i = 0; i < color_buttons.size(); ++i) {
        QPushButton* button = color_buttons[i];
        if (i == index)
            button->setStyleSheet(button->property("baseStyle").toString() + "QPushButton { border: 4px solid #000; }");
        else
            button->setStyleSheet(button->property("baseStyle").toString());
    }
    accept();
}

int ColorSelectionDialog::selectedColorIndex() const {
    return selected_index;
}

void ColorSelectionDialog::on_pushButton_red_clicked() {
    selectColor(0);
}

void ColorSelectionDialog::on_pushButton_blue_clicked() {
    selectColor(1);
}

void ColorSelectionDialog::on_pushButton_green_clicked() {
    selectColor(2);
}

void ColorSelectionDialog::on_pushButton_orange_clicked() {
    selectColor(3);
}

void ColorSelectionDialog::on_pushButton_purple_clicked() {
    selectColor(4);
}

void ColorSelectionDialog::on_pushButton_yellow_clicked() {
    selectColor(5);
}

void ColorSelectionDialog::on_pushButton_teal_clicked() {
    selectColor(6);
}

void ColorSelectionDialog::on_pushButton_pink_clicked() {
    selectColor(7);
}

void ColorSelectionDialog::on_pushButton_brown_clicked() {
    selectColor(8);
}