#ifndef ADDCHAT_H
#define ADDCHAT_H

#include <QDialog>

#include "xmppworker.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class AddChat;
}
QT_END_NAMESPACE

class AddChat: public QDialog
{
    Q_OBJECT

    private:
    Ui::AddChat *ui;
    XMPPWorker *worker;

    public:
    AddChat(XMPPWorker *worker, QWidget *parent = nullptr);
    ~AddChat();

    public slots:
    void onStartChat_clicked();

    signals:
    void newChat(const QString &barejid, const QString &message);

};

#endif // ADDCHAT_H