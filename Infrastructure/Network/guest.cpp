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
}