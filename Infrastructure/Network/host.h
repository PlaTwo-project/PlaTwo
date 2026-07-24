#ifndef HOST_H
#define HOST_H

#include <QObject>
#include "network.h"
#include <QTcpServer>
#include "Logic/User/user.h"
#include "Logic/Game/game_name.h"

class Host : public Network
{
    Q_OBJECT
public:
    explicit Host(QObject *parent = nullptr);
    ~Host();

    void startHosting(int port);
    QString getLocalIP() const;
    void sendRoomConfig(const User& host_user, int board_size, int time_limit, int host_color_index, int guest_color_index);
    void rejectGuest();

private slots:
    void connectNewClient();
    void handleIncomingData(const QByteArray &data); // 1 = creat room, 2 = guest join, 3 = move, 4 = resign, 5 = chat

signals:
    void guestJoined(const User& guest_user, int guest_color_index, GameName guest_game_name);
    void moveReceived(const QByteArray& moveData);

private:
    QTcpServer *server;
     int hosting_port;
};

#endif // HOST_H