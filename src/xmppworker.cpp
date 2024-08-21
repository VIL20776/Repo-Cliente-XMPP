#include "xmppworker.h"

#include "QXmppMessage.h"
#include "QXmppConfiguration.h"
#include "QXmppLogger.h"

#include <QDebug>

XMPPWorker::XMPPWorker(QObject *parent):
    QObject(parent),
    client(new QXmppClient(this))
{
    roster_manager = client->findExtension<QXmppRosterManager>();

    connect(client, &QXmppClient::messageReceived, this, &XMPPWorker::handleMessageReceived);
    connect(roster_manager, &QXmppRosterManager::rosterReceived, this, &XMPPWorker::handleRosterReceived);
    connect(roster_manager, &QXmppRosterManager::presenceChanged, this, &XMPPWorker::handlePresenceChanged);
}

void XMPPWorker::connectToServer() {
    client->logger()->setLoggingType(QXmppLogger::StdoutLogging);

    QXmppConfiguration config {};
    config.setJid("vil20776-test2@alumchat.lol");
    config.setPassword("Pipupo10182001");
    config.setIgnoreSslErrors(true);

    client->connectToServer(config);
}


void XMPPWorker::handleMessageReceived(const QXmppMessage &message) {
    QString from = message.from();
    QString body = message.body();

    if (message.type() == QXmppMessage::Chat)
        emit messageReceived(body);
}

void XMPPWorker::handlePresenceChanged(const QString &barejid, const QString &resource) {

    auto presence = roster_manager->getPresence(barejid, resource);

    QString status = presence.statusText();

    QString show; 
    
    switch (presence.availableStatusType())
    {
    case QXmppPresence::Online:
        show = "Online";
        break;
    case QXmppPresence::Away:
        show = "Away";
        break;
    case QXmppPresence::XA:
        show = "XA";
        break;
    case QXmppPresence::DND:
        show = "DND";
        break;
    case QXmppPresence::Chat:
        show = "Chat";
        break;
    case QXmppPresence::Invisible:
        show = "Invisible";
        break;
    default:
        break;
    }

    QString jid = barejid.split(u'/')[0];

    emit presenceChanged(jid, show + "\n" + status);
    
}

void XMPPWorker::handleRosterReceived() {
    const QStringList jids = roster_manager->getRosterBareJids();
    
    emit rosterReceived(jids);

}
