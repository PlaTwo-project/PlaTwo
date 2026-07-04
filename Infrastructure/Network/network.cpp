#include "network.h"

Network::Network(QObject *parent) : QObject(parent), socket(nullptr) {}

void Network::sendData(const QByteArray& data)
{
    if (socket && socket->isOpen()) {
        socket->write(data);
        socket->flush();
    }
}

void Network::stop()
{
    if (socket) {
        socket->close();
        socket->deleteLater();
        socket = nullptr;
    }
}

Network::~Network()
{
    stop();
}

void Network::initConnection()
{
    if (!socket)
        return;

    connect(socket, &QTcpSocket::readyRead, this, &Network::onReadyRead);

    connect(socket, &QTcpSocket::disconnected, this, &Network::onDisconnected);

    connect(socket, &QAbstractSocket::error, this, [this](){
        emit error(socket->errorString());
    });
}

void Network::onReadyRead()
{
    if (socket)
        emit dataReceived(socket->readAll());
}

void Network::onDisconnected()
{
    emit disconnected();
}