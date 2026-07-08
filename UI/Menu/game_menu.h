#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <QWidget>
#include "Logic/Game/game_name.h"

namespace Ui {
class GameMenu;
}

class GameMenu : public QWidget
{
    Q_OBJECT

public:
    explicit GameMenu(QWidget *parent = nullptr);
    void setTitle(GameName game_name);
    ~GameMenu();

private slots:
    void on_pushButton_start_game_clicked();
    void on_pushButton_history_clicked();
    void on_pushButton_back_clicked();

signals:
    void navigateToHostPage(GameName game_name);
    void navigateToGuestPage(GameName game_name);
    void navigateToHistory(GameName game_name);
    void navigateToMainMenu();

private:
    Ui::GameMenu *ui;
    GameName cur_game;
};

#endif // GAME_MENU_H
