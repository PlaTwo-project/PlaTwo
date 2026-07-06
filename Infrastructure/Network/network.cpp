#include "network.h"

Network::Network(QObject *parent) : QObject(parent), socket(nullptr) {}

void Network::sendData(const QByteArray &data)
{
    if (socket && socket->isOpen())
    {
        socket->write(data);
        socket->flush();
    }
}

Network::~Network()
{
    if (socket)
    {
        socket->close();
        socket->deleteLater();
        socket = nullptr;
    }
}

void Network::initConnection()
{
    if (!socket)
        return;

    connect(socket, &QTcpSocket::readyRead, this, &Network::readData);
    connect(socket, &QTcpSocket::disconnected, this, &Network::disconnectSocket);
    connect(socket, &QAbstractSocket::error, this, [this]() { emit error(socket->errorString()); });
}

void Network::readData()
{
    if (socket)
        emit dataReceived(socket->readAll());
}

void Network::disconnectSocket()
{
    emit disconnected();
}