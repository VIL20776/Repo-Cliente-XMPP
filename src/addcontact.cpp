#include "addcontact.h"
#include "ui_addcontact_dialog.h"

AddContact::AddContact(XMPPWorker *worker, QWidget *parent):
    QDialog(parent),
    ui(new Ui::AddContact),
    worker(worker)
{
    ui->setupUi(this);

    connect(ui->subscribe, &QPushButton::clicked, this, &AddContact::onSubscribe_clicked);
}

AddContact::~AddContact()
{

}

void AddContact::onSubscribe_clicked() {
    QDialog::accept();
}