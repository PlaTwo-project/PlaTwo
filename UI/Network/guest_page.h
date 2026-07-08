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
    void joinRequested(QString ip, QString port);

private:
    Ui::GuestPage *ui;
};

#endif // GUEST_PAGE_H
