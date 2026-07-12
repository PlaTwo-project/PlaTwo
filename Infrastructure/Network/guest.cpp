#include "guest.h"

Guest::Guest(QObject *parent): Network(parent) {
}

Guest::~Guest() {
}

void Guest::connectHost(const QString& IP, int port)
{
    socket = new QTcpSocket(this);

    initConnection();

    connect(socket, &QTcpSocket::connected, this, &Network::connected);

    socket->connectToHost(IP, port);

    connect(this, &Network::dataReceived, this, &Guest::handleIncomingData);
}

void Guest::sendGuestInfo(const User& guest_user) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << qint8(2) << guest_user.getName();
    sendData(block);
}

void Guest::handleIncomingData(const QByteArray &data) {
    QDataStream in(data);
    qint8 packet_type;
    in >> packet_type;

    if (packet_type == 1) {
        QString host_name;
        int board_size, time_limit;
        in >> host_name >> board_size >> time_limit;
        emit roomConfigReceived(User(host_name), board_size, time_limit);
    }

    if (packet_type == 3) {
        QByteArray move_data;
        in >> move_data;
        emit moveReceived(move_data);
    }
}