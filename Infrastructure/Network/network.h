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
    void sendData(const QByteArray& data);
    void stop();
    virtual ~Network();

signals:
    void connected();
    void dataReceived(const QByteArray& data);
    void disconnected();
    void error(const QString& error);

private slots:
    void onReadyRead();
    void onDisconnected();

protected:
    QTcpSocket* socket;
    void initConnection();
};

#endif // NETWORK_H
