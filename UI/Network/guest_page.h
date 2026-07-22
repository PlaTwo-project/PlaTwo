#ifndef GUEST_PAGE_H
#define GUEST_PAGE_H

#include <QWidget>
#include "Logic/Game/game_name.h"

namespace Ui {
class GuestPage;
}

class GuestPage : public QWidget
{
    Q_OBJECT

public:
    explicit GuestPage(QWidget *parent = nullptr);
    void setTitle(GameName game_name);
    void clearFields();
    ~GuestPage();

private slots:
    void on_pushButton_join_clicked();
    void on_pushButton_back_clicked();

signals:
    void navigateToGameMenu();
    void joinRequested(QString ip, int port, int colorIndex);

private:
    Ui::GuestPage *ui;
    GameName current_game_name = GameName::None;
};

#endif // GUEST_PAGE_H
