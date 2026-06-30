#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <QWidget>

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private slots:
    void on_pushButton_dots_clicked();
    void on_pushButton_morris_clicked();
    void on_pushButton_fanorona_clicked();
    void on_pushButton_history_clicked();
    void on_pushButton_editProfile_clicked();
    void on_pushButton_exit_clicked();
    void on_pushButton_logout_clicked();

signals:
    void navigateToEditProfile();
    void navigateToLogin();

private:
    Ui::MainMenu *ui;
};

#endif // MAIN_MENU_H
