#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <QWidget>

enum class GameName;

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
    void on_pushButton_editProfile_clicked();
    void on_pushButton_exit_clicked();
    void on_pushButton_logout_clicked();

signals:
    void navigateToGame1(GameName game_name);
    void navigateToGame2(GameName game_name);
    void navigateToGame3(GameName game_name);
    void navigateToEditProfile();
    void navigateToLogin();

private:
    Ui::MainMenu *ui;
};

#endif // MAIN_MENU_H
