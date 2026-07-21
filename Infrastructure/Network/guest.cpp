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
    out << qint8(2) << guest_user.getId() << guest_user.getName() << guest_user.getUsername();
    sendData(block);
}

void Guest::handleIncomingData(const QByteArray &data) {
    QDataStream in(data);
    qint8 packet_type;
    in >> packet_type;

    if (packet_type == 1) {
        int host_id;
        QString host_name, host_username;
        int board_size, time_limit;
        in >> host_id >> host_name >> host_username >> board_size >> time_limit;

        User host_user(host_name);
        host_user.setId(host_id);
        host_user.setUsername(host_username);
        emit roomConfigReceived(host_user, board_size, time_limit);
    }

    if (packet_type == 3) {
        QByteArray move_data;
        in >> move_data;
        emit moveReceived(move_data);
    }

    if (packet_type == 4)
        emit resignReceived();

    if (packet_type == 5) {
        QString chat_message;
        in >> chat_message;
        emit chatMessageReceived(chat_message);
    }
}