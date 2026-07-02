#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <QWidget>

enum class GameName { BoxesAndDots, NineMensMorris, Fanorona };

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
    void navigateToStartGame();
    void navigateToHistory();
    void navigateToMainMenu();

private:
    Ui::GameMenu *ui;
};

#endif // GAME_MENU_H
