#ifndef XMPP_CLIENT_H
#define XMPP_CLIENT_H

#include "QXmppClient.h"
#include "QXmppRosterManager.h"

// Clase que maneja las acciones del cliente
class XMPPWorker : public QObject
{
    Q_OBJECT

private:
    QString client_barejid;
    QXmppClient *client;
    QXmppRosterManager *roster_manager;
    
public:
    XMPPWorker(QObject *parent = nullptr);
    void connectToServer(QString jid, QString password);

    /*Recibe un mensaje del cliente y lo envía al servidor*/
    void handleSendMessage(const QString &message, const QString &to);
    /*Envía una solicitud de subscripción a un usuario*/
    void subscribe(const QString &to);
    /*Establece la presencia del cliente*/
    void setPresence(const int &type = 0, const QString &status = "");
    /*Retorna el JID del cliente*/
    QString getCurrentJID();
    /*Obtiene la presencia de un usuario. Requiere que se conozca el recurso*/
    QString getPresence(const QString &barejid, const QString &resource);

public slots: //Slots para manejar señales emitidas por la interfáz gráfica

    void handleMessageReceived(const QXmppMessage &message);
    void handlePresenceChanged(const QString &barejid, const QString &resource);

    void handleRosterReceived();
    void handleRosterItemAdded(const QString &barejid);

signals: //Señales emitidas a la interfaz gráfica

    void messageReceived(const QString &from, const QString &message);
    void rosterReceived(const QStringList &barejids);
    void presenceChanged(const QString &barejid, const QString &presence);
    void rosterItemAdded(const QString &barejid);

};

#endif // XMPP_CLIENT_H
