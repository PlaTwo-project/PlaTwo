#pragma once

#include <QByteArray>

class NetworkInterface
{
public:
    virtual ~NetworkInterface() = default;

    virtual void hostGame(int port) = 0;
    virtual void joinGame(const QString& ip, int port) = 0;
    virtual void sendData(const QByteArray& data) = 0;
    virtual bool isConnected() const = 0;
};