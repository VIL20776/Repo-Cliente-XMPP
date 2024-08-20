#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QXmppClient.h"
#include "QXmppMessage.h"

#include <QMainWindow>
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

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onMessageReceived(const QString &message);

signals:
    void startXMPPClient();

};
#endif // MAINWINDOW_H
