#ifndef XMPP_CLIENT_H
#define XMPP_CLIENT_H

#include "QXmppClient.h"
#include "QXmppRosterManager.h"

class XMPPWorker : public QObject
{
    Q_OBJECT

private:
    QString client_barejid;
    QXmppClient *client;
    QXmppRosterManager *roster_manager;
    
public:
    XMPPWorker(QObject *parent = nullptr);
    void connectToServer();

    void handleSendMessage(const QString &message, const QString &to);
    void subscribe(const QString &to);
    void setPresence(const int &type = 0, const QString &status = "");
    QString getCurrentJID();
    QString getPresence(const QString &barejid, const QString &resource);

public slots:
    void handleMessageReceived(const QXmppMessage &message);
    void handlePresenceChanged(const QString &barejid, const QString &resource);

    void handleRosterReceived();
    void handleRosterItemAdded(const QString &barejid);

signals:
    void messageReceived(const QString &from, const QString &message);
    void rosterReceived(const QStringList &barejids);
    void presenceChanged(const QString &barejid, const QString &presence);
    void rosterItemAdded(const QString &barejid);

};

#endif // XMPP_CLIENT_H
