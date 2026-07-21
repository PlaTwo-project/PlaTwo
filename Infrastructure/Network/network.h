#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>

class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = nullptr);
    virtual ~Network();
    
    void sendData(const QByteArray &data);

signals:
    void connected();
    void dataReceived(const QByteArray &data);
    void disconnected();
    void error(const QString &error);
    void resignReceived();

private slots:
    void readData();
    void disconnectSocket();

protected:
    QTcpSocket *socket;
    void initConnection();
};

#endif // NETWORK_H
