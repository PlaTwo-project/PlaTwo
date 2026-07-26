#include "network.h"
#include <QDataStream>
#include "Logic/Constants/packet_type.h"

Network::Network(QObject *parent) : QObject(parent), socket(nullptr) {}

void Network::sendData(const QByteArray &data) {
    if (socket && socket->isOpen()) {
        socket->write(data);
        socket->flush();
    }
}

void Network::sendChatMessage(const QString &message) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << static_cast<qint8>(PacketType::chat) << message;
    sendData(block);
}

Network::~Network() {
    if (socket) {
        socket->close();
        socket->deleteLater();
        socket = nullptr;
    }
}

void Network::initConnection() {
    if (!socket)
        return;

    connect(socket, &QTcpSocket::readyRead, this, &Network::readData);
    connect(socket, &QTcpSocket::disconnected, this, &Network::disconnectSocket);
    connect(socket, &QAbstractSocket::error, this, [this]() {
        emit error(socket->errorString());
    });
}

void Network::readData() {
    if (socket)
        emit dataReceived(socket->readAll());
}

void Network::disconnectSocket() {
    emit disconnected();
}

void Network::sendPauseRequest() {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << static_cast<qint8>(PacketType::PauseRequest);
    sendData(block);
}

void Network::sendPauseResponse(bool accepted) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << static_cast<qint8>(PacketType::PauseResponse) << accepted;
    sendData(block);
}