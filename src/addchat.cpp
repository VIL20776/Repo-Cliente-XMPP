#include "addchat.h"
#include "ui_addchat_dialog.h"

AddChat::AddChat(XMPPWorker *worker, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddChat),
    worker(worker)
{
    ui->setupUi(this);
    ui->message->setText("Hola, soy " + worker->getCurrentJID());
    
    connect(ui->start_chat, &QPushButton::clicked, this, &AddChat::onStartChat_clicked);
}

AddChat::~AddChat() = default;

void AddChat::onStartChat_clicked() {
    QString jid = ui->jid_input->text();
    if (jid == "") return;

    QString message = ui->message->text();

    worker->handleSendMessage(message, jid);
    emit newChat(jid, message);
    QDialog::accept();
}