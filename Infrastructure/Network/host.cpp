#include <QNetworkInterface>
#include <QHostAddress>
#include "host.h"

Host::Host(QObject *parent) : Network(parent), server(nullptr)
{
}

Host::~Host()
{
    if (server)
    {
        server->close();
        server->deleteLater();
    }
}

void Host::startHosting(int port)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &Host::connectNewClient);

    if (!server->listen(QHostAddress::Any, port))
        emit error(server->errorString());
}

void Host::connectNewClient()
{
    if (!server)
        return;

    socket = server->nextPendingConnection();
    server->close();

    initConnection();
    emit connected();

    connect(this, &Network::dataReceived, this, &Host::handleIncomingData);
}

QString Host::getLocalIP() const {
    QString local_ip = "127.0.0.1";
    const QList<QHostAddress> addresses = QNetworkInterface::allAddresses();

    for (const QHostAddress& address : addresses)
        if (address.protocol() == QAbstractSocket::IPv4Protocol && !address.isLoopback()) {
            local_ip = address.toString();
            break;
        }

    return local_ip;
}

void Host::sendRoomConfig(const User& host_user, int board_size, int time_limit) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << qint8(1) << host_user.getName() << host_user.getUsername() << board_size << time_limit;
    sendData(block);
}

void Host::handleIncomingData(const QByteArray &data) {
    QDataStream in(data);
    qint8 packet_type;
    in >> packet_type;

    if (packet_type == 2) {
        QString guest_name, guest_username;
        in >> guest_name >> guest_username;

        User guest_user(guest_name);
        guest_user.setUsername(guest_username);
        emit guestJoined(guest_user);
    }

    if (packet_type == 3) {
        QByteArray move_data;
        in >> move_data;
        emit moveReceived(move_data);
    }
}