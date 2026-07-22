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
    void sendChatMessage(const QString &message);
    void sendPauseRequest();
    void sendPauseResponse(bool accepted);

signals:
    void connected();
    void dataReceived(const QByteArray &data);
    void disconnected();
    void error(const QString &error);
    void resignReceived();
    void chatMessageReceived(const QString &message);
    void pauseRequested();
    void pauseResponded(bool accepted);

private slots:
    void readData();
    void disconnectSocket();

protected:
    QTcpSocket *socket;
    void initConnection();
};

#endif // NETWORK_H