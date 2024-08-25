#ifndef ADDCONTACT_H
#define ADDCONTACT_H

#include <QDialog>

#include "xmppworker.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class AddContact;
}
QT_END_NAMESPACE

class AddContact: public QDialog
{
    Q_OBJECT

    private:
    Ui::AddContact *ui;
    XMPPWorker *worker;

    public:
    AddContact(XMPPWorker *worker, QWidget *parent = nullptr);
    ~AddContact();

    public slots:
    void onSubscribe_clicked();

};

#endif // ADDCONTACT_H