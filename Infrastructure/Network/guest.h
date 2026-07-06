#ifndef GUEST_H
#define GUEST_H

#include "network.h"

    class Guest : public Network
{
    Q_OBJECT
public:
    explicit Guest(QObject *parent = nullptr);
    ~Guest();

    void connectHost(const QString& IP, int port);
};

#endif // GUEST_H