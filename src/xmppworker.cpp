#include "xmppworker.h"

#include "QXmppMessage.h"
#include "QXmppConfiguration.h"
#include "QXmppLogger.h"

#include <QDebug>

XMPPWorker::XMPPWorker(QObject *parent):
    QObject(parent),
    client(new QXmppClient(this))
{
    connect(client, &QXmppClient::messageReceived, this, &XMPPWorker::handleMessageReceived);
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

    client->sendPacket(QXmppMessage("", from, "Mensaje: " + body));

    emit messageReceived(body);
}
