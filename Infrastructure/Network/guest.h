#ifndef GUEST_H
#define GUEST_H

#include "network.h"
#include "Logic/User/user.h"
#include "Logic/Game/game_name.h"

    class Guest : public Network
{
    Q_OBJECT
public:
    explicit Guest(QObject *parent = nullptr);
    ~Guest();

    void connectHost(const QString& IP, int port);
    void sendGuestInfo(const User& guest_user, int guest_color_index, GameName game_name);

signals:
    void roomConfigReceived(const User& host_user, int board_size, int time_limit, int host_color_index, int guest_color_index);
    void moveReceived(const QByteArray& moveData);

private slots:
    void handleIncomingData(const QByteArray &data); // 1 = creat room, 2 = guest join, 3 = move, 4 = resign, 5 = chat
};

#endif // GUEST_H