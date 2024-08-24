#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QXmppClient.h"
#include "QXmppMessage.h"

#include <QMainWindow>
#include <QListWidgetItem>
#include <QThread>

#include "xmppworker.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    XMPPWorker *worker;
    QThread *client_thread;

    QMap<QString, QListWidgetItem*> contact_map;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onMessageReceived(const QString &from, const QString &message);
    void onRosterReceived(const QStringList &barejids);
    void onPresenceChanged(const QString &barejid, const QString &presence);
    void onChatList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void onSendButton_clicked();

signals:
    void startXMPPClient();

};
#endif // MAINWINDOW_H
