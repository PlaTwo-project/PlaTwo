#ifndef COLOR_SELECTION_DIALOG_H
#define COLOR_SELECTION_DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVector>

namespace Ui {
class ColorSelectionDialog;
}

class ColorSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColorSelectionDialog(QWidget* parent = nullptr);
    int selectedColorIndex() const;

private slots:
    void on_pushButton_red_clicked();
    void on_pushButton_blue_clicked();
    void on_pushButton_green_clicked();
    void on_pushButton_orange_clicked();
    void on_pushButton_purple_clicked();
    void on_pushButton_yellow_clicked();
    void on_pushButton_teal_clicked();
    void on_pushButton_pink_clicked();
    void on_pushButton_brown_clicked();

private:
    void selectColor(int index);

    Ui::ColorSelectionDialog *ui;
    QVector<QPushButton*> color_buttons;
    int selected_index;
};

#endif // COLOR_SELECTION_DIALOG_H