#ifndef XMPP_CLIENT_H
#define XMPP_CLIENT_H

#include "QXmppClient.h"

class XMPPWorker : public QObject
{
    Q_OBJECT
private:
    QXmppClient *client;
    
public:
    XMPPWorker(QObject *parent = nullptr);
    void connectToServer();

public slots:
    void handleMessageReceived(const QXmppMessage &message);

signals:
    void messageReceived(const QString &message);

};

#endif // XMPP_CLIENT_H
