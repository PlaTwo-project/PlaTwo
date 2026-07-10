#ifndef HOST_PAGE_H
#define HOST_PAGE_H

#include <QWidget>
#include "Logic/Game/game_name.h"

namespace Ui {
class HostPage;
}

class HostPage : public QWidget
{
    Q_OBJECT

public:
    explicit HostPage(QWidget *parent = nullptr);
    ~HostPage();

    void setPage(GameName game_name);
    void switchToWaitingStatus(const QString& Ip);

signals:
    void createRoomRequested(int port, int boardSize, int timeLimitMinutes);
    void navigateToGameMenu();
    void cancelHostRequested();

private slots:
    void on_pushButton_create_clicked();
    void on_pushButton_back_clicked();
    void on_pushButton_cancelHost_clicked();

private:
    Ui::HostPage *ui;
};

#endif // HOST_PAGE_H