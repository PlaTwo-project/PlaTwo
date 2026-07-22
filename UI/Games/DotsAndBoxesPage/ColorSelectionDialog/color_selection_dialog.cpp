#include "color_selection_dialog.h"
#include "ui_color_selection_dialog.h"
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesColors/dots_and_boxes_colors.h"

#include <QButtonGroup>

ColorSelectionDialog::ColorSelectionDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ColorSelectionDialog), selected_index(-1) {
    ui->setupUi(this);
}

ColorSelectionDialog::~ColorSelectionDialog() {
    delete ui;
}

void ColorSelectionDialog::selectColor(int index) {
    selected_index = index;
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