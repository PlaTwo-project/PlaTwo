#ifndef GUEST_H
#define GUEST_H

#include "network.h"
#include "Logic/User/user.h"

    class Guest : public Network
{
    Q_OBJECT
public:
    explicit Guest(QObject *parent = nullptr);
    ~Guest();

    void connectHost(const QString& IP, int port);
    void sendGuestInfo(const User& guest_user);

signals:
    void roomConfigReceived(const User& host_user, int board_size, int time_limit);
    void moveReceived(const QByteArray& moveData);

private slots:
    void handleIncomingData(const QByteArray &data);

};

#endif // GUEST_H