#include "xmppworker.h"

#include "QXmppMessage.h"
#include "QXmppConfiguration.h"
#include "QXmppLogger.h"

#include <QDebug>

XMPPWorker::XMPPWorker(QObject *parent):
    QObject(parent),
    client(new QXmppClient(this))
{
    setPresence();
    roster_manager = client->findExtension<QXmppRosterManager>();

    connect(client, &QXmppClient::messageReceived, this, &XMPPWorker::handleMessageReceived);
    connect(roster_manager, &QXmppRosterManager::rosterReceived, this, &XMPPWorker::handleRosterReceived);
    connect(roster_manager, &QXmppRosterManager::itemAdded, this, &XMPPWorker::handleRosterItemAdded);
    connect(roster_manager, &QXmppRosterManager::presenceChanged, this, &XMPPWorker::handlePresenceChanged);
}

void XMPPWorker::connectToServer(QString jid, QString password) {
    client->logger()->setLoggingType(QXmppLogger::StdoutLogging);

    client_barejid = jid;
    QXmppConfiguration config {};
    config.setJid(client_barejid);
    config.setPassword(password);
    config.setIgnoreSslErrors(true);

    client->connectToServer(config);
}

void XMPPWorker::handleSendMessage(const QString &message, const QString &to) {
    client->sendPacket(QXmppMessage("", to, message));
}

void XMPPWorker::subscribe(const QString &to) {
    roster_manager->subscribe(to);
}

void XMPPWorker::setPresence(const int &type, const QString &status) {
    using StatusType = QXmppPresence::AvailableStatusType;

    QXmppPresence my_presence = {};
    my_presence.setAvailableStatusType((StatusType) type);
    
    if (status != "")
        my_presence.setStatusText(status);

    client->setClientPresence(my_presence);
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

QString XMPPWorker::getCurrentJID() {
    return client_barejid;
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

void XMPPWorker::handleRosterItemAdded(const QString &barejid) {
    emit rosterItemAdded(barejid);
}
