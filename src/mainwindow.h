#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QXmppClient.h"
#include "QXmppMessage.h"

#include <QMainWindow>
#include <QListWidgetItem>
#include <QThread>

#include "xmppworker.h"
#include "addcontact.h"
#include "addchat.h"
#include "signon.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// Ventana principal de la aplicación
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    XMPPWorker *worker;
    QThread *client_thread;

    // Ventanas auxiliares

    AddContact *add_contact;
    AddChat *add_chat;
    SignOn *signon;

    QMap<QString, QListWidgetItem*> contact_map;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots: //Slots para manejar acciones del usuario y señales del cliente 

    void onMessageReceived(const QString &from, const QString &message);
    void onRosterReceived(const QStringList &barejids);
    void onRosterItemAdded(const QString &barejid);
    void onPresenceChanged(const QString &barejid, const QString &presence);
    void onNewChat(const QString &barejid, const QString &message = "");
    void onChatList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void onUpdatePresece_clicked();
    void onAddContact_clicked();
    void onAddChat_clicked();
    void onSendButton_clicked();

};
#endif // MAINWINDOW_H
