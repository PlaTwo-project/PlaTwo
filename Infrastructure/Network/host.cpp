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
}