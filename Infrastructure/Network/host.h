#ifndef HOST_H
#define HOST_H

#include <QObject>
#include "network.h"
#include <QTcpServer>
#include "Logic/User/user.h"

class Host : public Network
{
    Q_OBJECT
public:
    explicit Host(QObject *parent = nullptr);
    ~Host();

    void startHosting(int port);
    QString getLocalIP() const;
    void sendRoomConfig(const User& host_user, int board_size, int time_limit);

private slots:
    void connectNewClient();
    void handleIncomingData(const QByteArray &data);

signals:
    void guestJoined(const User& guest_user);
    void moveReceived(const QByteArray& moveData);

private:
    QTcpServer *server;
};

#endif // HOST_H