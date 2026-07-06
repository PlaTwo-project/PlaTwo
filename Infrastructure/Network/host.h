#ifndef HOST_H
#define HOST_H

#include <QObject>
#include "network.h"
#include <QTcpServer>

class Host : public Network
{
    Q_OBJECT
public:
    explicit Host(QObject *parent = nullptr);
    ~Host();

    void startHosting(int port);

private slots:
    void connectNewClient();

private:
    QTcpServer *server;
};

#endif // HOST_H