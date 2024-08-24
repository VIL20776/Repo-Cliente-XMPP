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

void XMPPWorker::handleSendMessage(const QString &message, const QString &to) {
    client->sendPacket(QXmppMessage("", to, message));
}

QString XMPPWorker::getPresence(const QString &barejid, const QString &resource) {
    QXmppPresence presence = roster_manager->getPresence(barejid, resource);

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

    return  show + "\n" + status;
}

void XMPPWorker::handleMessageReceived(const QXmppMessage &message) {
    QString from = message.from();
    QString body = message.body();
    
    if (body == "")
        return;

    switch (message.type())
    {
        case QXmppMessage::Chat:
            emit messageReceived(from.split(u'/').at(0), body);
        

    }
}

void XMPPWorker::handlePresenceChanged(const QString &barejid, const QString &resource) {

    QString presence = getPresence(barejid, resource);
    QString jid = barejid.split(u'/')[0];

    emit presenceChanged(jid, presence);
    
}

void XMPPWorker::handleRosterReceived() {
    const QStringList jids = roster_manager->getRosterBareJids();
    
    emit rosterReceived(jids);

}
