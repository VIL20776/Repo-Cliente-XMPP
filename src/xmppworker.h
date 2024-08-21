#ifndef XMPP_CLIENT_H
#define XMPP_CLIENT_H

#include "QXmppClient.h"
#include "QXmppRosterManager.h"

class XMPPWorker : public QObject
{
    Q_OBJECT

    struct ContactInfo {
        QString availableStatus;
        QString status;
    };

private:
    QXmppClient *client;
    QXmppRosterManager *roster_manager;
    QMap<QString, ContactInfo> contacts;
    QMap<QString, QStringList> chats;
    
public:
    XMPPWorker(QObject *parent = nullptr);
    void connectToServer();

public slots:
    void handleMessageReceived(const QXmppMessage &message);
    void handlePresenceChanged(const QString &barejid, const QString &resource);
    void handleRosterReceived();

signals:
    void messageReceived(const QString &from, const QString &message);
    void rosterReceived(const QStringList &barejids);
    void presenceChanged(const QString &barejid, const QString &presence);

};

#endif // XMPP_CLIENT_H
